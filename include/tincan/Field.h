#ifndef FIELD_H__
#define FIELD_H__

#include <string>
#include <iostream>

namespace tincan
{

// TODO: think about foreign keys. Option::Reference, target type passing.

/** Field options bit field values. */
struct FieldOptions
{
    static const int PrimaryKey = (1 << 0);
    static const int Unique     = (1 << 1);
    static const int Null       = (1 << 2);
};

/**
 * Field is a descriptor that provides metadata (like label, type, constraints
 * etc) for an underlying POD type.
 *
 * It functions as proxy for the underlying object via assignment and cast
 * operators.
 */
template <typename T, const char* lbl, int opts = 0>
struct Field
{
    typedef T type;

    std::string label;
    int options;

    Field(const T& value = T()) :
        label(lbl),
        options(opts),
        _value(value)
    {}

    operator T& () { return _value; }
    operator const T& () const { return _value; }
    const T& value() const { return _value; }

    T& operator = (const T& rhs) {
        _value = rhs;
        return _value;
    }

    friend std::ostream& operator << (std::ostream& out,
            const Field<T, lbl, opts>& f)
    {
        out << f._value;
        return out;
    }

    friend std::istream& operator >> (std::istream& in,
            Field<T, lbl, opts>& f)
    {
        in >> f._value;
        return in;
    }

private:
    T _value;
};

}

#endif /* FIELD_H */
