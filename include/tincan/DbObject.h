#ifndef DATAOBJECT_H__
#define DATAOBJECT_H__

#include "PreparedStatement.h"

#include "disable_copy.h"

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

template <typename T>
const std::string& DbFieldType(int fieldOptions);

class DbFieldDeclarationBuilder
{
public:
    DbFieldDeclarationBuilder(std::ostringstream& out) :
        _out(out)
    {}

    template <class TypedField>
    void operator<< (const TypedField& field)
    {
        _out << field.label
            << " "
            << DbFieldType<typename TypedField::type>(field.options)
            << ",";
    }

private:
    DISABLE_COPY(DbFieldDeclarationBuilder)

    std::ostringstream& _out;
};

class DbFieldBinder
{
public:
    DbFieldBinder(PreparedStatement::ptr& statement) :
        _statement(statement),
        _counter(1)
    {}

    template <class TypedField>
    void operator<< (const TypedField& field)
    {
        _statement->bind<typename TypedField::type>(_counter, field.value());
        _counter++;
    }

private:
    DISABLE_COPY(DbFieldBinder)

    PreparedStatement::ptr& _statement;
    unsigned int _counter;
};

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

    DbObject(const object_ptr& obj) :
        object(obj),
        _save_statement(),
        _load_statement()
    {}

    DbObject(UnderlyingObject* obj) :
        object(obj),
        _save_statement(),
        _load_statement()
    {}

    const object_ptr& operator -> () const { return object; }

    object_ptr& operator -> () { return object; }

    operator bool () const { return object; }

    // TODO: SQL is SQLite-specific, refactor if other backends required
    std::string createTableStatement() const
    {
        std::ostringstream sql;
        sql << "CREATE TABLE " << object->metainfo.label << " (";
        sql << object->id.label
            << " INTEGER PRIMARY KEY AUTOINCREMENT,";

        DbFieldDeclarationBuilder fb(sql);
        object->acceptRead(fb);

        std::string ret = sql.str();
        ret.erase(ret.end() - 1); // remove last comma

        return ret + ");";
    }

    void save()
    {
        /*

        // FIXME: primary key < 1 ? INSERT : UPDATE

        if (!_save_statement) {
            std::ostringstream sql;
            sql << "INSERT INTO " << object->metainfo.label;

            std::ostringstream columns;
            std::ostringstream fieldPlaceholders;

            DbInsertQueryBuilder qb(columns, fieldPlaceholders);
            object->acceptRead(qb);

            std::string s = columns.str();
            s.erase(ret.end() - 1); // remove last comma

            sql << "(" << s << ")";

            s = fieldPlaceholders.str();
            s.erase(ret.end() - 1);

            sql << "values (" << s << ")";

            _save_statement.reset(PreparedStatementFactory::create());
        }

        DbFieldBinder fb(_save_statement);
        object->acceptRead(fb);

        _save_statement->executeUpdate();

        */
    }

private:
    // TODO: caching statements may be problematic in embedded environments
    // (well, at least they are lazy-loaded)
    PreparedStatement::ptr _save_statement;
    PreparedStatement::ptr _load_statement;
};

}

#endif /* DATAOBJECT_H */
