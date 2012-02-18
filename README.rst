tincan: a C++ object mapper
===========================

At the core of ``tincan`` is a simple generic framework of fields and metadata
for mapping objects to other data formats.

On top of that, ``tincan`` provides:

* the SQL aspect: a simple ORM for SQLite databases.

* the XML aspect: serialization to/from XML.

``tincan`` is `licenced under the MIT licence`_ - feel free to modify and
use it commercially.

Quick overview::

 tincan::DbObject<Person> ervin(new Person("Ervin", 38));

 // fields provide metadata for and proxy the underlying type
 std::cout << ervin->name.label << ": " << ervin->name << ", ";
 std::cout << ervin->age.label  << ": " << ervin->age << std::endl;
 // -> output: name: Ervin, age: 38

 // DbObject-specific aspect: SQL statement building
 std::cout << ervin.createTableStatement() << std::endl;
 // -> output: CREATE TABLE person (id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT,age INT);

 // DbObject-specific aspect: store object in database
 ervin.save();

 tincan::DbObject<Person> jill;

 // DbObject-specific aspect: load object from database by field
 jill.load(jill->name, "Jill");

 // wrap the same object with XML decorator
 tincan::XmlObject<Person> xjill(jill);

 // XmlObject-specific aspect: serialize to XML
 std::cout << xjill.toXml() << std::endl;

See `main test`_ for more details.

.. _`licenced under the MIT licence`: https://github.com/mrts/tincan/blob/master/LICENCE.rst
.. _main test: https://github.com/mrts/tincan/blob/master/test/src/main.cpp
