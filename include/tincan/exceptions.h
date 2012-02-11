#ifndef EXCEPTIONS_H__
#define EXCEPTIONS_H__

#include <stdexcept>

namespace tincan
{

class SQLException : public std::runtime_error
{
public:
    SQLException(const char* msg) :
        std::runtime_error(msg)
    {}
};

}

#endif /* EXCEPTIONS_H */
