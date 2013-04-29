#ifndef ARG3_CPARSE_TYPE_DATE_H_
#define ARG3_CPARSE_TYPE_DATE_H_

#include <ctime>
#include <string>
#include "../value.h"

namespace cparse
{
    namespace type
    {
        class Date
        {
        public:
            Date(const std::string &date);
            Date(time_t value);
            Date();

            Value toValue() const;

            time_t getTime() const;

            string getTimeString() const;

        protected:
            static const char *const FORMAT;

            static time_t convert(const std::string &value);

        private:
            time_t value_;
        };
    }
}

#endif
