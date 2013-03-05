#include <assert.h>
#include <time.h>
#include <stdio.h>

time_t cparse_date_time(const char *s)
{
	struct tm tp;

	strptime(s, "%FT%T%z", &tp);

	return mktime(&tp);
}
