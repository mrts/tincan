#include "tincan/DbException.h"

#include <sstream>

namespace tincan
{

std::string DbException::buildMessage(const std::string& msg,
        const char* function,
        const PreparedStatement::ptr& statement)
{
    std::ostringstream ss;
    ss << msg << " in " << function
        << ". SQL statement: " << statement->getSQL();

    return ss.str();
}

}
