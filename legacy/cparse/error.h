#ifndef CPARSE_ERROR_H_
#define CPARSE_ERROR_H_

#include <cparse/defines.h>

#ifdef __cplusplus
extern "C" {
#endif

struct cparse_error
{
	int code;
	char *message;
};

CParseError *cparse_error_new();

void cparse_error_free(CParseError *error);


#ifdef __cplusplus
}
#endif

#endif
