#ifndef FIELD_H__
#define FIELD_H__

#include <string>

namespace tincan
{

template<typename T>
class Field
{
public:
    Field(const std::string& name, const T& value) :
        _name(name), _value(value)
    {}

    const std::string& type() const;

    std::string sqlDeclaration() const
    {
        return _name + " " + type();
    }

private:
    std::string _name;
    const T& _value;
};

}

#endif /* FIELD_H */
