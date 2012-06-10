#ifndef DATAOBJECT_H__
#define DATAOBJECT_H__

#include "db/FieldVisitors.h"
#include "dbccpp/PreparedStatement.h"

#include "detail/stream_exceptions.h"

// don't depend on boost when using C++11
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
  #include <memory>
#else
  #include <boost/smart_ptr/shared_ptr.hpp>
#endif

#include <string>
#include <sstream>

namespace tincan
{

template <class UnderlyingObject>
class DbObject
{
public:
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
    typedef std::shared_ptr<UnderlyingObject>    object_ptr;
#else
    typedef boost::shared_ptr<UnderlyingObject>  object_ptr;
#endif

    object_ptr object;

    DbObject(UnderlyingObject* obj) :
        object(obj),
        _insert_statement(),
        _update_statement(),
        _load_statement()
    {}

    const object_ptr& operator -> () const { return object; }

    // TODO: excellent oportunity for dirty status tracking:
    // don't allow modification of fields directly but use
    // .modify()-> ... instead. modify() would return normal reference.
    object_ptr& operator -> () { return object; }

    operator bool () const { return object; }

    // SQL statements
    // TODO: SQL is SQLite-specific, refactor if other backends required

    std::string createTableStatement() const
    {
        std::ostringstream sql;
        sql << "CREATE TABLE " << object->metainfo.label << " (";
        sql << object->id.label
            << " INTEGER PRIMARY KEY AUTOINCREMENT,";

        DbFieldDeclarationBuilder fb(sql);
        object->acceptRead(fb);

        long pos = sql.tellp();
        sql.seekp(pos - 1); // remove last comma

        sql << ");";

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

        sql << " VALUES (" << s << ");";

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

        sql << " WHERE id = ?;";

        return sql.str();
    }

    void save()
    {
        // TODO: dirty status tracking
        // if _not_dirty: return
        dbc::PreparedStatement::ptr statement;
        bool update = object->id > 0;

        if (update) {
            prepareUpdateStatement();
            statement = _update_statement;
        } else {
            // insert
            prepareInsertStatement();
            statement = _insert_statement;
        }

        DbFieldBinder fb(statement);
        object->acceptRead(fb);

        if (update)
            // update needs to to have id bound as well
            fb << object->id;

        int howmany = statement->executeUpdate();
        if (howmany != 1) {
            std::ostringstream msg;
            msg << "PreparedStatement::executeUpdate() returned "
                << howmany << "instead of 1";
            throw DbException(msg.str(), __FUNCTION__); // , statement);
        }

        if (!update) {
            // insert needs to set the object id after insert

            // NOTE: the result of this call is unpredictable
            // if a separate thread performs a new INSERT on
            // the same database connection while getLastInsertId()
            // is executing.
            object->id = statement->getLastInsertId();
        }
    }

private:
    // TODO: caching so many statements may be problematic
    // in embedded environments
    // (well, at least they are lazy-loaded)
    dbc::PreparedStatement::ptr _insert_statement;
    dbc::PreparedStatement::ptr _update_statement;
    dbc::PreparedStatement::ptr _load_statement;

    inline void prepareInsertStatement()
    {
        /*
        if (!_insert_statement)
            _insert_statement.reset(
                    PreparedStatement::create(insertStatement());
                    PreparedStatementFactory::create(insertStatement()));
        */
    }

    inline void prepareUpdateStatement()
    {
        /*
        if (!_update_statement)
            _insert_statement.reset(
                    PreparedStatementFactory::create(updateStatement()));
        */
    }
};

}

#endif /* DATAOBJECT_H */
