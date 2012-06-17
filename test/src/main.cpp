#include "Person.h"

#include <testcpp/testcpp.h>

#include <tincan/tincan.h>

#include <string>
#include <vector>
#include <iostream>
#include <exception>

void example()
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

    /*
    DbObject<Person> p1 =
        DbObject<Person>::loadById(1);

    std::cout << p1->name.label << ": " << p1->name << std::endl;

    DbObject<Person> p2 =
        DbObject<Person>::loadByField<std::string>("name", "Ervin");

    std::cout << p2->name.label << ": " << p2->name << std::endl;
    */

    // TODO: loading collections
}

int main()
{
    try
    {
        example();

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
