tincan: a C++ ORM for SQLite
============================

Quick overview::

 tincan::DataObject<Person> po(new Person("Ervin", 38));
 std::cout << po.object->name;

 Person& p = *po.object;
 std::cout << " " << p.age << std::endl;

 std::cout << po.createTable() << std::endl;

 po.save();
