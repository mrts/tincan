#include <tincan.h>

#include <string>
#include <vector>
#include <iostream>

class Person
{
public:
    Person(const std::string& n, int a) :
        name(n),
        age(a),
        address()
    {}

    // public fields
    std::string name;
    int age;
    std::vector<std::string> address;

    // accessor methods for generic visitors
    template <class Visitor>
    void accessFields(Visitor& v)
    {
        v
        << tincan::Field<std::string>("name", name)
        << tincan::Field<int>("age", age);
        // will support:
        //  constraints  - tincan::Field::Unique
        //  foreing keys - tincan::Field::ForeignKey<OtherClass>
        //  etc
    }

    template <class Visitor>
    void accessMetainfo(Visitor& v)
    {
        // v << tincan::Table("Person");
    }
};


int main()
{
    // Need to take ownership of the object, so using scoped pointers and
    // heap objects. Would *love* to use && instead.
    // Perhaps make ownership explicit with auto_ptr?
    tincan::DataObject<Person> p(new Person("Mart", 38));
    std::cout << p.object->name;

    Person& p2 = *p.object;
    std::cout << " " << p2.age << std::endl;

    std::cout << p.createTable() << std::endl;

    p.save();

    /*
    p.object->m_name = "Kalle";
    p.object->m_age = 27;
    p.save();

    p.load("name", "Mart");
    std::cout << p.object.m_age << std::endl;

    p.load(tincan::Query::StartsWith("name", "A") &
            tincan::Query::Equals("age", 30));
    std::cout << p.object.m_age << std::endl;
    */

    return 0;
}
