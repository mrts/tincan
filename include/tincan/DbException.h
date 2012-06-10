#ifndef EXCEPTIONS_H__
#define EXCEPTIONS_H__

#include <stdexcept>
#include <string>

namespace dbc { class PreparedStatement; }

namespace tincan
{

class DbException : public std::runtime_error
{
public:
    DbException(const std::string& msg, const char* function,
        const dbc::PreparedStatement& statement) :
        std::runtime_error(buildMessage(msg, function, statement))
    {}

private:
    std::string buildMessage(const std::string& msg, const char* function,
            const dbc::PreparedStatement& statement);
};

}

#endif /* EXCEPTIONS_H */
