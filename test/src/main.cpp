#include <tincan/tincan.h>

#include <string>
#include <vector>
#include <iostream>
#include <exception>

struct Person
{
    Person(const std::string& n, int a) :
        id(-1),
        name(n),
        age(a)
    {}

    // TODO:
    //  namespaces  - tincan::Namespace<TRIP>
    //  combined constraints - unique together etc
    //  in some far future - inheritance
    static const char PERSON_CLASS[];
    static tincan::Metainfo<PERSON_CLASS> metainfo;

    static const char ID_FIELD[];
    /** id is a special "primary key" field that must be present for the db
     * ascpect. (You get a compile error when using DbObject if it is not.)
     * It shouldn't be passed to visitors as it is accessed directly.
     */
    tincan::Field<int, ID_FIELD, tincan::FieldOptions::PrimaryKey> id;

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

// metainfo definition
const char Person::PERSON_CLASS[] = "person";
tincan::Metainfo<Person::PERSON_CLASS> Person::metainfo;

// field names definitions
const char Person::ID_FIELD[] = "id";
const char Person::NAME_FIELD[] = "name";
const char Person::AGE_FIELD[] = "age";

int main()
{
    try {
        // DB connection is a singleton and needs to be configured before use
        // Currently SQLite-specific, but ready for other backends too with
        // little refactoring
        // tincan::DbConnection& db = tincan::DbConnection::instance();
        // db.connect("test.db");

        tincan::DbObject<Person> ervin(new Person("Ervin", 38));

        // fields provide metadata for and proxy the underlying type
        std::cout << ervin->name.label << ": " << ervin->name << ", ";
        std::cout << ervin->age.label  << ": " << ervin->age << std::endl;

        ervin->age = 39;
        std::cout << ervin->age.label  << ": " << ervin->age << std::endl;

        // DbObject-specific aspect: SQL statement building
        std::cout << ervin.createTableStatement() << std::endl;
        std::cout << ervin.insertStatement() << std::endl;
        std::cout << ervin.updateStatement() << std::endl;

        // DbObject-specific aspect: create the database table
        // ervin.createTable();

        // DbObject-specific aspect: store object in database
        // std::cout << ervin->id.label << ": " ervin->id << std::endl;
        // ervin.save();
        // std::cout << ervin->id.label << ": " ervin->id << std::endl;

    } catch (const std::exception& e) {

        std::cerr << e.what() << std::endl;

    }

    return 0;
}
