#include <stdlib.h>
#include <stdio.h>
#include <cparse/error.h>

CParseError *cparse_error_new()
{
	CParseError *e = malloc(sizeof(CParseError));

	if(e == NULL) {
		fputs("Unable to allocate memory", stderr);
		exit(EXIT_FAILURE);
	}

	e->code = 0;
	e->message = NULL;

	return e;
}

void cparse_error_free(CParseError* e) {
	if(e->message)
		free(e->message);

	free(e);
}
