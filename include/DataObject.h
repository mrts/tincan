#ifndef DATAOBJECT_H__
#define DATAOBJECT_H__

#include "scoped_pointer.h"

#include <sstream>

namespace tincan
{

class FieldBuilder
{
public:
    FieldBuilder(std::stringstream& out) :
        _out(out)
    {}

    template <class TypedField>
    FieldBuilder& operator<< (const TypedField& field)
    {
        _out << field.sqlDeclaration() << ",";
        return *this;
    }

private:
    std::stringstream& _out;
};

template <class UnderlyingObject>
class DataObject
{
public:
    typedef UnderlyingObject value_type;

    scoped_pointer<UnderlyingObject> object;

    // Takes ownership of the object
    DataObject(UnderlyingObject* obj) :
        object(obj)
    {}

    std::string createTable()
    {
        std::stringstream sql;
        // TODO: use object.accessMetainfo(fb) instead for table name
        sql << "CREATE TABLE " << "<<FIXME>>" <<
            " (id INTEGER PRIMARY KEY AUTOINCREMENT,";

        FieldBuilder fb(sql);
        object->accessFields(fb);

        std::string ret = sql.str();
        ret.erase(ret.end() - 1); // remove last comma

        return ret + ");";
    }

    void save()
    {
        // not implemented, see createTable() for how it works
    }
};

}

#endif /* DATAOBJECT_H */
