#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <curl/curl.h>
#include <json/json.h>
#include <cparse/object.h>
#include <cparse/error.h>
#include "io.h"

const char *const cparse_domain = "https://api.parse.com";
const char *const cparse_api_version = "1";

extern const char *cparse_api_key;

extern const char *cparse_app_id;

CParseRequest *cparse_request_new()
{
    CParseRequest *request = malloc(sizeof(CParseRequest));

    request->path = NULL;
    request->payload = NULL;
    request->method = kCPRequestGet;

    return request;
};

void cparse_request_free(CParseRequest *request)
{
    if(request->path)
        free(request->path);
    if(request->payload)
        free(request->payload);

    free(request);
}

static size_t cparse_io_get_response(void *ptr, size_t size, size_t nmemb, CParseResponse *s)
{
    assert(s != NULL);

    size_t new_len = s->size + size * nmemb;
    s->text = realloc(s->text, new_len + 1);
    if (s->text == NULL)
    {
        fputs("realloc() failed", stderr);
        exit(EXIT_FAILURE);
    }
    memcpy(s->text + s->size, ptr, size * nmemb);
    s->text[new_len] = '\0';
    s->size = new_len;

    return size * nmemb;
}

static void cparse_io_set_request_url(CURL *curl, const char *path)
{
    char buf[BUFSIZ + 1];

    snprintf(buf, BUFSIZ, "%s/%s/%s", cparse_domain, cparse_api_version, path);

    curl_easy_setopt(curl, CURLOPT_URL, buf);
}

static void cparse_io_set_headers(CURL *curl)
{
    assert(cparse_app_id != NULL);

    char buf[BUFSIZ + 1];

    struct curl_slist *headers = NULL;

    snprintf(buf, BUFSIZ, "X-Parse-Application-Id: %s", cparse_app_id);

    headers = curl_slist_append(headers, buf);

    snprintf(buf, BUFSIZ, "X-Parse-REST-API-Key: %s", cparse_api_key);

    headers = curl_slist_append(headers, buf);

    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

CParseObject *cparse_io_request_json(CParseRequest *request, CParseError **error)
{
    CParseResponse *response = cparse_io_perform(request);

    json_object *jobj = json_tokener_parse(response->text);

    CParseObject *obj = cparse_object_from_json(jobj);

    char *errorMessage = cparse_object_get_string(obj, "error");

    if(errorMessage != NULL) {
        *error = cparse_error_new();

        (*error)->message = strdup(errorMessage);

        (*error)->code = cparse_object_get_number(obj, "code", 0);

        cparse_object_free(obj);

        return NULL;
    }

    return obj;
}
CParseResponse *cparse_io_perform(CParseRequest *request)
{
    CURL *curl;
    CURLcode res;
    CParseResponse *response;

    curl = curl_easy_init();
    if (curl == NULL)
    {
        fputs("unable to init curl", stderr);
        return NULL;
    }

    response = malloc(sizeof(CParseResponse));
    response->text = NULL;
    response->code = 0;
    response->size = 0;

    switch (request->method)
    {
    case kCPRequestPost:
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request->payload);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request->payload_size);
        break;
    case kCPRequestPut:
        curl_easy_setopt(curl, CURLOPT_PUT, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request->payload);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request->payload_size);
        break;
    case kCPRequestDelete:
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        break;
    default:
        break;
    }

    cparse_io_set_request_url(curl, request->path);
    cparse_io_set_headers(curl);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cparse_io_get_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
        fputs("problem requesting with curl", stderr);

    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &response->code);

    /* always cleanup */
    curl_easy_cleanup(curl);

    return response;
}

