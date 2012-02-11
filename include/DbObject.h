#ifndef DATAOBJECT_H__
#define DATAOBJECT_H__

// FIXME: #ifdef USE_BOOST, else use C++11 std::shared_ptr
#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>
#include <sstream>

namespace tincan
{

template <typename T>
const std::string& DbFieldType();

class DbFieldBuilder
{
public:
    DbFieldBuilder(std::ostringstream& out) :
        _out(out)
    {}

    template <class TypedField>
    void operator<< (const TypedField& field)
    {
        _out << field.label
            << " "
            << DbFieldType<typename TypedField::type>()
            << ",";
    }

private:
    std::ostringstream& _out;
};

template <class UnderlyingObject>
class DbObject
{
public:
    typedef boost::shared_ptr<UnderlyingObject> object_type;

    object_type object;

    DbObject(const object_type& obj) :
        object(obj)
    {}

    DbObject(UnderlyingObject* obj) :
        object(obj)
    {}

    const object_type& operator -> () const { return object; }

    object_type& operator -> () { return object; }

    operator bool () const { return object; }

    std::string createTableStatement() const
    {
        std::ostringstream sql;
        sql << "CREATE TABLE " << object->metainfo.label <<
            " (id INTEGER PRIMARY KEY AUTOINCREMENT,";

        DbFieldBuilder fb(sql);
        object->acceptRead(fb);

        std::string ret = sql.str();
        ret.erase(ret.end() - 1); // remove last comma

        return ret + ");";
    }

    void save()
    {
        // not implemented, see createTable() for the approach
    }
};

}

#endif /* DATAOBJECT_H */
