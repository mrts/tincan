#ifndef PERSON_H__
#define PERSON_H__

#include <tincan/Metainfo.h>
#include <tincan/Field.h>

#include <string>

// TODO:
//  namespaces  - tincan::Namespace<TRIP>
//  combined constraints - unique together etc
//  in some far future - inheritance

struct Person
{
    Person(const std::string& n = "", int a = 0) :
        id(-1),
        name(n),
        age(a)
    {}

    static const char PERSON_CLASS[];
    static tincan::Metainfo<PERSON_CLASS> metainfo;

    static const char ID_FIELD[];
    /** id is a special "primary key" field that must be present for the db
     * ascpect. (You get a compile error when using DbObject if it is not.)
     * It shouldn't be passed to visitors as it is accessed directly.
     */
    tincan::Field<int, ID_FIELD> id;

    static const char NAME_FIELD[];
    tincan::Field<std::string, NAME_FIELD> name;

    static const char AGE_FIELD[];
    tincan::Field<int, AGE_FIELD> age;

    /** Let generic visitors read fields. */
    template <class Visitor>
    void acceptRead(Visitor& v)
    {
        v << name;
        v << age;
    }

    /** Let generic visitors write fields. */
    template <class Visitor>
    void acceptWrite(Visitor& v)
    {
        v >> name;
        v >> age;
    }
};

#endif /* PERSON_H */
