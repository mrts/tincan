#ifndef DATAOBJECT_H__
#define DATAOBJECT_H__

// don't depend on boost when using C++11
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
  #include <memory>
  #pragma message "Using std::shared_ptr"
#else
  #include <boost/smart_ptr/shared_ptr.hpp>
  #pragma message "Using boost::shared_ptr"
#endif

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
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus > 199711L)
    typedef std::shared_ptr<UnderlyingObject>   object_type;
#else
    typedef boost::shared_ptr<UnderlyingObject> object_type;
#endif

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
