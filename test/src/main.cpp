#include <tincan.h>

#include <string>
#include <vector>
#include <iostream>

struct Person
{
    Person(const std::string& n, int a) :
        name(n),
        age(a)
    {}

    // TODO:
    //  namespaces  - tincan::Namespace<TRIP>
    //  combined constraints - unique together etc
    //  in some far future - inheritance
    static const char PERSON[];
    tincan::Metainfo<PERSON> metainfo;

    // TODO:
    //  constraints  - tincan::Field::Unique
    //  foreing keys - tincan::Field::ForeignKey<OtherClass>
    //  etc
    static const char NAME[];
    tincan::Field<std::string, NAME> name;

    static const char AGE[];
    tincan::Field<int, AGE> age;

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

// class and field names
const char Person::PERSON[] = "person";
const char Person::NAME[] = "name";
const char Person::AGE[] = "age";


int main()
{
    tincan::DbObject<Person> ervin(new Person("Ervin", 38));

    // fields provide metadata for and proxy the underlying type
    std::cout << ervin->name.label << ": " << ervin->name << ", ";
    std::cout << ervin->age.label  << ": " << ervin->age << std::endl;

    ervin->age = 39;
    std::cout << ervin->age.label  << ": " << ervin->age << std::endl;

    // DbObject-specifi aspect: SQL statement building
    std::cout << ervin.createTableStatement() << std::endl;

    // DbObject-specifi aspect: store object in database
    // ervin.save();

    /*
    p.load(tincan::Query::StartsWith(p->name, "A") &
            tincan::Query::Equals("age", 30));
    std::cout << p.object.m_age << std::endl;
    */

    return 0;
}
