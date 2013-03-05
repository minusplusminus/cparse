#ifndef CPARSE_IO_H_
#define CPARSE_IO_H_

#include <stdlib.h>
#include <cparse/defines.h>

#define MAX_PAYLOAD_SIZE 10240

typedef enum
{
	kCPRequestGet,
	kCPRequestPost,
	kCPRequestPut,
	kCPRequestDelete
} CParseRequestMethod;

struct cparse_io_response
{
	char *text;
	size_t size;
	int code;
};

struct cparse_io_request
{
	char *path;
	char *payload;
	size_t payload_size;
	CParseRequestMethod method;
};

typedef struct cparse_io_response CParseResponse;

typedef struct cparse_io_request CParseRequest;

CParseResponse *cparse_io_perform(CParseRequest *request);

CParseRequest *cparse_request_new();

void cparse_request_free(CParseRequest *request);

CParseObject *cparse_io_request_json(CParseRequest *request, CParseError **error);

#endif
