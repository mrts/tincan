#include "tincan/DbObject.h"
#include "tincan/Field.h"
#include "tincan/release_assert.h"

namespace tincan
{

//TODO: these declarations are SQLite-specific, refactor if other backends
// required

// FIXME: most options have no effect currently

template <typename T>
const std::string& DbFieldType(int)
{
    RELEASE_ASSERT(false, "Unknown field type");

    static const std::string ret("");
    return ret;
}

template <>
const std::string& DbFieldType<std::string>(int fieldOptions)
{
    RELEASE_ASSERT(!fieldOptions, "Options haven't been implemented yet");

    static const std::string ret("TEXT");
    return ret;
}

template <>
const std::string& DbFieldType<int>(int fieldOptions)
{
    static std::string ret;

    if (fieldOptions == FieldOptions::PrimaryKey)
        ret = "INTEGER PRIMARY KEY AUTOINCREMENT";
    else if (fieldOptions)
        RELEASE_ASSERT(false, "Only primary key implemented currently");
    else
        ret = "INT";

    return ret;
}

}
