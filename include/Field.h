#ifndef FIELD_H__
#define FIELD_H__

#include <string>
#include <iostream>

namespace tincan
{

/**
 * Field is a descriptor that provides metadata (like label, type, constraints
 * etc) for an underlying POD type.
 *
 * It functions as proxy for the underlying object via assignment and cast
 * operators.
 */
template <typename T, const char* l>
struct Field
{
    typedef T type;
    std::string label;

    Field(const T& value) :
        label(l),
        _value(value)
    {}

    operator T& () { return _value; }
    operator const T& () const { return _value; }

    T& operator = (const T& rhs) {
        _value = rhs;
        return _value;
    }

    friend std::ostream& operator << (std::ostream& out, const Field<T, l>& f)
    {
        out << f._value;
        return out;
    }

    friend std::istream& operator >> (std::istream& in, Field<T, l>& f)
    {
        in >> f._value;
        return in;
    }

private:
    T _value;
};

}

#endif /* FIELD_H */
