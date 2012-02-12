#ifndef EXCEPTIONS_H__
#define EXCEPTIONS_H__

#include "db/PreparedStatement.h"

#include <stdexcept>
#include <string>

namespace tincan
{

class DbException : public std::runtime_error
{
public:
    DbException(const std::string& msg, const char* function,
        const PreparedStatement::ptr& statement) :
        std::runtime_error(buildMessage(msg, function, statement))
    {}

private:
    std::string buildMessage(const std::string& msg, const char* function,
            const PreparedStatement::ptr& statement);
};

}

#endif /* EXCEPTIONS_H */
