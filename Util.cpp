#include <ctime>
#include <string>

namespace cparse
{

time_t datetime(const std::string &s)
{
	struct tm tp;

	strptime(s.c_str(), "%FT%T%z", &tp);

	return mktime(&tp);
}

}

