#include "Person.h"

// metainfo definition
const char Person::PERSON_CLASS[] = "person";
tincan::Metainfo<Person::PERSON_CLASS> Person::metainfo;

// field name definitions
const char Person::ID_FIELD[] = "id";
const char Person::NAME_FIELD[] = "name";
const char Person::AGE_FIELD[] = "age";
