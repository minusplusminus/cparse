#ifndef ARG3_CPARSE_EXCEPTION_H
#define ARG3_CPARSE_EXCEPTION_H

#include <exception>
#include <string>

namespace cparse
{
    class Exception : public std::logic_error
    {
    public:
        Exception(const std::string &what) : std::logic_error(what)
        {}
    };
}

#endif
