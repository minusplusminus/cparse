#include <ctime>
#include <string>

namespace cparse
{

    time_t datetime(const std::string &s)
    {
        struct tm tp;

        if (!strptime(s.c_str(), "%FT%T%z", &tp))
            return 0;

        return mktime(&tp);
    }

}

