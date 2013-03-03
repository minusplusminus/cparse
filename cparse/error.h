#ifndef CPARSE_ERROR_H_
#define CPARSE_ERROR_H_

#include <cparse/defines.h>

struct cparse_error
{
	int code;
	char *message;
};

CParseError *cparse_error_new();

void cparse_error_free(CParseError *error);

#endif
