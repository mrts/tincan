#include "../include/Field.h"

namespace
{

// As template specialization only works in namespace level in C++03,
// these functions have to be freestanding, although conceptually they belong
// to the Field class

template <typename T>
const std::string& _fieldtype()
{
    static const std::string ret("## unknown field type ##");
    return ret;
}

template <>
const std::string& _fieldtype<std::string>()
{
    static const std::string ret("TEXT");
    return ret;
}

template <>
const std::string& _fieldtype<int>()
{
    static const std::string ret("INT");
    return ret;
}

}

namespace tincan
{

template <typename T>
const std::string& Field<T>::type() const
{
    return _fieldtype<T>();
}

template class Field<int>;
template class Field<std::string>;

}
