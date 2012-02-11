#include "../include/DbObject.h"
#include "../include/release_assert.h"

namespace tincan
{

template <typename T>
const std::string& DbFieldType()
{
    RELEASE_ASSERT(false, "Unknown field type");

    static const std::string ret("");
    return ret;
}

template <>
const std::string& DbFieldType<std::string>()
{
    static const std::string ret("TEXT");
    return ret;
}

template <>
const std::string& DbFieldType<int>()
{
    static const std::string ret("INT");
    return ret;
}

}
