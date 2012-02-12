#include "tincan/DbObject.h"
#include "tincan/Field.h"
#include "tincan/release_assert.h"

namespace tincan
{

// TODO: SQL is SQLite-specific, refactor if other backends required

// FIXME: options have no effect currently

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
    RELEASE_ASSERT(!fieldOptions, "Options haven't been implemented yet");

    static std::string ret("INT");
    return ret;
}

}
