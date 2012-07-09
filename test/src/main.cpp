#include "Person.h"

#include <testcpp/testcpp.h>

#include <tincan/tincan.h>

#include <pugixml/pugixml.hpp>

#include <string>
#include <iostream>
#include <exception>
#include <vector>

void print_person(tincan::DbObject<Person>& obj)
{
    std::cout << obj->name << std::endl;
}

void db_example()
{
    using namespace tincan;

    // need to connect to a database before using DbObjects
    dbc::DbConnection::connect("sqlite", "test.db");

    // always use make_shared() when creating shared pointers
    DbObject<Person> ervin(stdutil::make_shared<Person>("Ervin", 38));

    // slight inconvenience - an object is needed for creating database tables
    ervin.createTable();

    // fields provide metadata for and proxy the underlying type
    std::cout << ervin->id.label  << ": " << ervin->id << ", ";
    std::cout << ervin->name.label << ": " << ervin->name << ", ";
    std::cout << ervin->age.label  << ": " << ervin->age << std::endl;

    ervin->age = 39;
    std::cout << ervin->age.label  << ": " << ervin->age << std::endl;

    ervin.save();
    std::cout << ervin->id.label  << ": " << ervin->id << std::endl;

    DbObject<Person> marvin(stdutil::make_shared<Person>("Marvin", 42));
    marvin.save();

    DbObject<Person> p;

    p.loadById(1);

    std::cout << p->name.label << ": " << p->name << std::endl;

    // true = allow many results, the first one is picked arbitrarily
    p.loadByField<std::string>("name", "Ervin", true);

    std::cout << p->name.label << ": " << p->name << std::endl;

    dbc::PreparedStatement::ptr statement =
        dbc::DbConnection::instance().prepareStatement("SELECT * FROM person");

    p.loadByQuery(statement, true);

    std::cout << p->name.label << ": " << p->name << std::endl;

    std::vector<DbObject<Person> > persons;
    DbObject<Person>::loadManyByQuery(statement, persons);

    std::for_each(persons.begin(), persons.end(), print_person);

    DbObject<Person>::loadManyByField<std::string>("name", "%vin", persons);

    std::for_each(persons.begin(), persons.end(), print_person);

    // TODO: add loading collections by return for RVO-aware compilers
    /*
    std::vector<DbObject<Person> > persons =
        DbObject<Person>::loadManyByField<std::string>("name", "%vin");

    std::vector<DbObject<Person> > persons =
        DbObject<Person>::loadManyByQuery(...);
    */
}

void xml_example()
{
    using namespace tincan;
    using namespace pugi;

    xml_document doc;
    if (!doc.load("<Person name=\"Ervin\" />"))
        throw std::runtime_error("XML parsing failed");

    /*
    XmlObject<Person> ervin(stdutil::make_shared<Person>("Ervin", 38));

    std::cout << ervin.toXml() << std::endl;
    std::cout << ervin.toXmlFragment() << std::endl;
    */


    /*
    // fields provide metadata for and proxy the underlying type
    std::cout << ervin->id.label  << ": " << ervin->id << ", ";
    std::cout << ervin->name.label << ": " << ervin->name << ", ";
    std::cout << ervin->age.label  << ": " << ervin->age << std::endl;

    ervin->age = 39;
    std::cout << ervin->age.label  << ": " << ervin->age << std::endl;

    ervin.save();
    std::cout << ervin->id.label  << ": " << ervin->id << std::endl;

    DbObject<Person> marvin(stdutil::make_shared<Person>("Marvin", 42));
    marvin.save();

    DbObject<Person> p;

    p.loadById(1);
    */

    // TODO: support arbitrary XPath queries to get DbObject-like
    // loadManyByQuery and loadByQuery. pugixml has good support for XPath.

}

int main()
{
    try
    {
        db_example();
        xml_example();

        /*
         * TODO: create a proper test suite ASAP
         *
        Test::assertEqual<std::string>("field labels work", ...);
        Test::assertEqual<std::string>("field values work", ...);
        Test::assertEqual<std::string>("changing fields works", ...);

        Test::assertEqual<std::string>("create table statement",
                ervin.createTableStatement(),
                "CREATE TABLE person "
                "(id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT,age INT);");

        Test::assertEqual<std::string>("insert statement",
                ervin.insertStatement(),
                "INSERT INTO person (name,age) VALUES (?,?);");

        Test::assertEqual<std::string>("update statement",
                ervin.updateStatement(),
                "UPDATE person SET name = ?,age = ? WHERE id = ?;");
        */

    }
    catch (const std::exception& e)
    {
        std::cerr << "EXCEPTION OCCURED: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
