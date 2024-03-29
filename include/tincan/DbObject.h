#ifndef DATAOBJECT_H__
#define DATAOBJECT_H__

#include "db/FieldVisitors.h"
#include "db/DbException.h"

#include <dbccpp/PreparedStatement.h>
#include <dbccpp/DbConnection.h>

#include "detail/stream_exceptions.h"

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
  #include <memory>
  #include <functional>
  namespace stdutil = std;
#else
  #include <boost/smart_ptr/shared_ptr.hpp>
  #include <boost/make_shared.hpp>
  #include <boost/function.hpp>
  namespace stdutil = boost;
#endif

#include <string>
#include <vector>
#include <sstream>

namespace tincan
{

template <class T>
class DbObject
{
public:
    typedef stdutil::shared_ptr<T> object_ptr;

    object_ptr object;

    DbObject() : object(stdutil::make_shared<T>()) {}

    DbObject(const object_ptr& obj) : object(obj) {}

    const object_ptr& operator-> () const { return object; }

    // TODO: excellent oportunity for dirty status tracking:
    // don't allow modification of fields directly but use
    // .modify()-> ... instead. modify() would return normal reference.
    object_ptr& operator-> () { return object; }

    operator bool () const { return object; }

    // SQL statements
    // TODO: SQL is SQLite-specific, refactor dbc-cpp if other backends required
    // TODO: move statements to impl files

    std::string createTableStatement() const
    {
        std::ostringstream sql;
        ENABLE_EXCEPTIONS(sql);

        sql << "CREATE TABLE IF NOT EXISTS " << object->metainfo.label << " (";
        sql << object->id.label
            << " INTEGER PRIMARY KEY AUTOINCREMENT,";

        DbFieldDeclarationBuilder fb(sql);
        object->acceptRead(fb);

        long pos = sql.tellp();
        sql.seekp(pos - 1); // remove last comma

        sql << ")";

        return sql.str();
    }

    std::string insertStatement() const
    {
        std::ostringstream sql;
        ENABLE_EXCEPTIONS(sql);

        sql << "INSERT INTO " << object->metainfo.label << " ";

        std::ostringstream columnLabels;
        ENABLE_EXCEPTIONS(columnLabels);
        std::ostringstream fieldPlaceholders;
        ENABLE_EXCEPTIONS(fieldPlaceholders);

        DbInsertQueryFieldBuilder iqfb(columnLabels, fieldPlaceholders);
        object->acceptRead(iqfb);

        std::string s = columnLabels.str();
        s.erase(s.end() - 1); // remove last comma

        sql << "(" << s << ")";

        s = fieldPlaceholders.str();
        s.erase(s.end() - 1);

        sql << " VALUES (" << s << ")";

        return sql.str();
    }

    std::string updateStatement() const
    {
        std::ostringstream sql;
        ENABLE_EXCEPTIONS(sql);
        sql << "UPDATE " << object->metainfo.label << " SET ";

        DbUpdateQueryFieldBuilder uqfb(sql);
        object->acceptRead(uqfb);

        long pos = sql.tellp();
        sql.seekp(pos - 1); // remove last comma

        sql << " WHERE id = ?";

        return sql.str();
    }

    std::string loadByIdStatement() const
    {
        return loadByFieldStatement("id");
    }

    std::string loadByFieldStatement(const std::string& field) const
    {
        std::ostringstream sql;
        ENABLE_EXCEPTIONS(sql);

        sql << "SELECT * FROM " << object->metainfo.label
            << " WHERE " << field << " = ?";

        return sql.str();
    }

    void createTable()
    {
        std::string sql(createTableStatement());
        dbc::DbConnection::instance().executeUpdate(sql.c_str());
    }

    void save()
    {
        // TODO: dirty status tracking
        // if _not_dirty: return
        dbc::PreparedStatement::ptr statement;
        bool update = object->id > 0;

        if (update)
        {
            prepareUpdateStatement();
            statement = _update_statement;
        }
        else
        {
            prepareInsertStatement();
            statement = _insert_statement;
        }

        DbFieldBinder fb(statement);
        object->acceptRead(fb);

        if (update)
            // update needs to to have id bound as well
            fb << object->id;

        int howmany = statement->executeUpdate();
        if (howmany != 1)
        {
            std::ostringstream msg;
            msg << "PreparedStatement::executeUpdate() returned "
                << howmany << "instead of 1";
            throw DbException(msg.str(), __FUNCTION__, *statement);
        }

        if (!update)
        {
            // insert needs to set the object id after insert

            // NOTE: the result of this call is unpredictable
            // if a separate thread performs a new INSERT on
            // the same database connection while getLastInsertId()
            // is executing.
            object->id = statement->getLastInsertId();
        }
    }

    void loadById(int id)
    {
        // FIXME: fix exceptions
        if (id < 1)
            throw DbException("id cannot be < 1", __FUNCTION__,
                    *_load_by_id_statement);

        object->id = id;

        prepareStatement(_load_by_id_statement, &DbObject::loadByIdStatement);
        _load_by_id_statement->set<int>(1, id);

        dbc::ResultSet::ptr result(_load_by_id_statement->executeQuery());
        result->next();
        bindFields(result, *this);

        if (result->next())
            throw DbException("More than one result for id", __FUNCTION__,
                    *_load_by_id_statement);
    }

    template <typename FieldType>
    void loadByField(const std::string& field, FieldType value,
            bool allowMany = false)
    {
        dbc::PreparedStatement::ptr statement = dbc::DbConnection::instance()
                .prepareStatement(loadByFieldStatement(field));
        statement->set<FieldType>(1, value);

        loadByQuery(statement, allowMany);
    }

    template <typename FieldType>
    static void loadManyByField(const std::string& field, FieldType value,
            std::vector<DbObject<T> >& out)
    {
        // TODO: should we out.clear() first?
        // TODO: accessing metainfo through temporary objects is unclean...
        DbObject<T> tmp;
        dbc::PreparedStatement::ptr statement = dbc::DbConnection::instance()
                .prepareStatement(tmp.loadByFieldStatement(field));
        statement->set<FieldType>(1, value);

        loadManyByQuery(statement, out);
    }

    // TODO: carefully consider the safety implications of taking
    // shared_ptrs by ref in a public API
    void loadByQuery(const dbc::PreparedStatement::ptr& statement,
            bool allowMany = false)
    {
        dbc::ResultSet::ptr result(statement->executeQuery());
        result->next();
        bindFields(result, *this);

        if (!allowMany && result->next())
            throw DbException("More than one result", __FUNCTION__, *statement);
    }

    static void loadManyByQuery(const dbc::PreparedStatement::ptr& statement,
            std::vector<DbObject<T> >& out)
    {
        dbc::ResultSet::ptr result(statement->executeQuery());

        while (result->next())
        {
            DbObject<T> obj;
            bindFields(result, obj);

            out.push_back(obj);
        }
    }

private:
    static dbc::PreparedStatement::ptr _insert_statement;
    static dbc::PreparedStatement::ptr _update_statement;
    static dbc::PreparedStatement::ptr _load_by_id_statement;

    inline void prepareStatement(dbc::PreparedStatement::ptr& statement,
            stdutil::function<std::string (DbObject<T>*)> createSql)
    {
        if (!statement)
        {
            statement = dbc::DbConnection::instance()
                .prepareStatement(createSql(this));
        }
        else
        {
            statement->reset();
            statement->clear();
        }
    }

    inline void prepareInsertStatement()
    {
        prepareStatement(_insert_statement, &DbObject::insertStatement);
    }

    inline void prepareUpdateStatement()
    {
        prepareStatement(_update_statement, &DbObject::updateStatement);
    }

    inline static void bindFields(dbc::ResultSet::ptr& result, DbObject<T>& obj)
    {
        obj->id = result->get<int>(0);

        ObjectFieldBinder fieldbinder(result);
        obj->acceptWrite(fieldbinder);
    }

};

template<typename T>
dbc::PreparedStatement::ptr DbObject<T>::_insert_statement;

template<typename T>
dbc::PreparedStatement::ptr DbObject<T>::_update_statement;

template<typename T>
dbc::PreparedStatement::ptr DbObject<T>::_load_by_id_statement;

}

#endif /* DATAOBJECT_H */
