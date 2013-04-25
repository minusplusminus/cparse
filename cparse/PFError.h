#ifndef ARG3_CPARSE_ERROR_H
#define ARG3_CPARSE_ERROR_H

#include <exception>
#include <string>

namespace cparse
{
    class PFError : public std::exception
    {
    public:
    PFError(int code, const std::string &message) noexcept : code_(code), message_(message)
        {

        }

    PFError(const PFError &error) noexcept : code_(error.code_), message_(error.message_)
        {

        }

        virtual ~PFError()
        {

        }

        PFError &operator=(const PFError &other) noexcept
        {
            if (this != &other)
            {
                code_ = other.code_;
                message_ = other.message_;
            }
            return *this;
        }

        int code() const
        {
            return code_;
        }

        const char *what() const noexcept
        {
            return message_.c_str();
        }
    private:
        int code_;
        std::string message_;
    };
}

#endif
