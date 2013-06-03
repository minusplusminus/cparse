#ifndef CPARSE_IO_H_
#define CPARSE_IO_H_

#include <stdlib.h>
#include <cparse/defines.h>

typedef enum
{
    kHTTPRequestGet,
    kHTTPRequestPost,
    kHTTPRequestPut,
    kHTTPRequestDelete
} HTTPRequestMethod;

struct cparse_client_response
{
    char *text;
    size_t size;
    int code;
};

struct cparse_client_request
{
    char *path;
    char *payload;
    HTTPRequestMethod method;
};

typedef struct cparse_client_response CParseClientResponse;

typedef struct cparse_client_request CParseClientRequest;

CParseClientResponse *cparse_client_request_get_response(CParseClientRequest *request);

CParseClientRequest *cparse_client_request_new();

void cparse_client_request_free(CParseClientRequest *request);

void cparse_client_response_free(CParseClientResponse *response);

CParseJSON *cparse_client_request_get_json(CParseClientRequest *request, CParseError **error);

void cparse_client_request_perform(CParseClientRequest *request, CParseError **error);

#endif
