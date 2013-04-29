#include <cparse/type/date.h>
#include <cparse/exception.h>
#include "../protocol.h"

namespace cparse
{
    namespace type
    {
        time_t Date::convert(const std::string &s)
        {
            struct tm tp;

            if (!strptime(s.c_str(), FORMAT, &tp))
                throw Exception("unable to convert time string");

            return mktime(&tp);
        }

        const char *const Date::FORMAT = "%FT%T%z";

        Date::Date() : value_(time(0))
        {}

        Date::Date(time_t value) : value_(value)
        {}

        Date::Date(const std::string &value) : value_(convert(value))
        {}

        Value Date::toValue() const {
            Value value;

            value.setString(protocol::KEY_TYPE, protocol::TYPE_DATE);

            value.setString("iso", getTimeString());

            return value;
        }

        time_t Date::getTime() const {
            return value_;
        }

        std::string Date::getTimeString() const
        {
            char buf[BUFSIZ+1] = {0};

            strftime(buf, BUFSIZ, FORMAT, localtime(&value_));

            return buf;
        }
    }
}