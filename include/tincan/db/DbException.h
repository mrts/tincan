#ifndef EXCEPTIONS_H__
#define EXCEPTIONS_H__

#include <stdexcept>
#include <string>
#include <sstream>

#include <dbccpp/PreparedStatement.h>

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
    inline std::string buildMessage(const std::string& msg,
            const char* function,
            const dbc::PreparedStatement& statement)
    {
        std::ostringstream ss;
        ss << msg << " in " << function
            << ". SQL statement: " << statement.getSQL();

        return ss.str();
    }
};

}

#endif /* EXCEPTIONS_H */
