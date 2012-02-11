#ifndef RELEASE_ASSERT_H__
#define RELEASE_ASSERT_H__

#include <stdexcept>
#include <string>

#define RELEASE_ASSERT(cond, msg) \
    if (!(cond)) { \
        std::string fn(__FUNCTION__); \
        std::string m(msg);\
        std::string c(#cond); \
        throw std::runtime_error(fn + ": " + c + ": " + m); \
    } else (void) 0

#endif /* RELEASE_ASSERT_H */
