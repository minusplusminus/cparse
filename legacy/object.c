#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cparse/object.h>
#include <cparse/error.h>
#include <cparse/json.h>
#include <stdio.h>
#include "io.h"
#include <cparse/util.h>
#include <json/json_object_iterator.h>

/*
 * this should be prime
 */
#define ATTRIBUTE_STARTSIZE 1021

#define ACTIVE 1

/* internals */

/* for background threads */
typedef struct cparse_object_background_arg
{
    CParseObject *obj;
    CParseObjectCallback callback; /* the callback passed by user */
    bool (*action)(CParseObject *obj, CParseError **error); /* the method to call in thread */
    pthread_t thread;

} CParseObjectBackgroundArg;

/* this is a background thread. The argument controlls functionality*/
void *cparse_object_background_action(void *argument)
{
    CParseError *error = NULL;
    CParseObjectBackgroundArg *arg = (CParseObjectBackgroundArg*) argument;

    /* cparse_object_save or cparse_object_refresh */
    (*arg->action)(arg->obj, &error);

    if(arg->callback) {
        (*arg->callback)(arg->obj, error);
    }

    free(arg);

    return NULL;
}

/* initializers */
static CParseObject *cparse_object_new()
{
	CParseObject *obj = malloc(sizeof(CParseObject));

	obj->className = NULL;
	obj->objectId = NULL;
	obj->acl = NULL;
	obj->createdAt = 0;
	obj->updatedAt = 0;
    obj->attributes = cparse_json_new();

	return obj;
}

CParseObject *cparse_object_copy(CParseObject *other)
{
    CParseObject *obj = cparse_object_new();

    obj->className = strdup(other->className);
    obj->objectId = strdup(other->objectId);
    obj->acl = cparse_acl_copy(other->acl);
    obj->createdAt = other->createdAt;
    obj->updatedAt = other->updatedAt;

    cparse_object_merge_json(obj, other->attributes);

    return obj;
}

CParseObject *cparse_object_with_class_name(const char *className)
{
	CParseObject *obj = cparse_object_new();

	obj->className = strdup(className);

	return obj;
}

CParseObject *cparse_object_with_class_data(const char *className, CParseJSON* attributes)
{
	CParseObject *obj = cparse_object_with_class_name(className);

	cparse_object_merge_json(obj, attributes);

	return obj;
}

/* cleanup */
void cparse_object_free(CParseObject *obj) {

    cparse_json_free(obj->attributes);

	if(obj->className)
		free(obj->className);
	if(obj->objectId)
		free(obj->objectId);
	free(obj);
}

/* io */

bool cparse_object_refresh(CParseObject *obj, CParseError **error)
{
    CParseRequest *request;
    CParseJSON *response;
    char buf[BUFSIZ+1];

    if(!obj->objectId || !*obj->objectId) {
        return false;
    }

    request = cparse_request_new();

    /* build the request */
    snprintf(buf, BUFSIZ, "classes/%s/%s", obj->className, obj->objectId);

    request->path = strdup(buf);

    request->method = kCPRequestGet;

    /* do the deed */
    response = cparse_request_get_json(request, error);

    cparse_request_free(request);

    if(error != NULL && *error != NULL)
    {
        return false;
    }

    /* merge the response with the object */
    cparse_object_merge_json(obj, response);

    return true;
}

pthread_t cparse_object_refresh_in_background(CParseObject *obj, CParseObjectCallback callback)
{
    assert(obj != NULL);
    CParseObjectBackgroundArg *arg = malloc(sizeof(CParseObjectBackgroundArg));

    arg->action = cparse_object_refresh;
    arg->obj = obj;
    arg->callback = callback;

    int rc = pthread_create(&arg->thread, NULL, cparse_object_background_action, arg);
    assert(rc == 0);

    return arg->thread;
}

bool cparse_object_save(CParseObject *obj, CParseError **error)
{
	CParseJSON *response;
	CParseRequest *request = cparse_request_new();
	char buf[BUFSIZ+1];

    /* build the request based on the id */
	if(!obj->objectId || !*obj->objectId) {
		snprintf(buf, BUFSIZ, "classes/%s", obj->className);

		request->path = strdup(buf);

		request->method = kCPRequestPost;
	}
	else {
		snprintf(buf, BUFSIZ, "classes/%s/%s", obj->className, obj->objectId);

		request->path = strdup(buf);

		request->method = kCPRequestPut;
	}

    /* build the json payload */

	request->payload = strdup(cparse_json_to_json_string(obj->attributes));

    /* do the deed */
	response = cparse_request_get_json(request, error);

    cparse_request_free(request);

	if(error != NULL && *error != NULL) {
		return false;
	}

    /* merge the result with the object */
    cparse_object_merge_json(obj, response);

	return true;
}

pthread_t cparse_object_save_in_background(CParseObject *obj, CParseObjectCallback callback)
{
    assert(obj != NULL);
    CParseObjectBackgroundArg *arg = malloc(sizeof(CParseObjectBackgroundArg));

    arg->action = cparse_object_save;
    arg->obj = obj;
    arg->callback = callback;

    int rc = pthread_create(&arg->thread, NULL, cparse_object_background_action, arg);
    assert(rc == 0);

    return arg->thread;
}

/* setters */

void cparse_object_set_number(CParseObject *obj, const char *key, long long value)
{
	assert(obj != NULL);

	cparse_object_set(obj, key, cparse_json_new_number(value));
}

void cparse_object_set_real(CParseObject *obj, const char *key, double value)
{
	assert(obj != NULL);

	cparse_object_set(obj, key, cparse_json_new_real(value));
}
void cparse_object_set_bool(CParseObject *obj, const char *key, bool value)
{
    assert(obj != NULL);

    cparse_object_set(obj, key, cparse_json_new_bool(value));
}

void cparse_object_set_string(CParseObject *obj, const char *key, const char *value)
{
	assert(obj != NULL);

	cparse_object_set(obj, key, cparse_json_new_string(value));
}

void cparse_object_set(CParseObject *obj, const char *key, CParseJSON *value)
{
	assert(obj != NULL);

	cparse_json_set(obj->attributes, key, cparse_json_new_reference(value));
}


void cparse_object_foreach_attribute(CParseObject * obj, void (*foreach) (CParseJSON *data))
{
    json_object_object_foreach(obj->attributes, key, val)
    {
        foreach(val);
    }
}

CParseJSON *cparse_object_remove(CParseObject * obj, const char* key)
{
    return cparse_json_remove(obj->attributes, key);
}

/* getters */

CParseJSON *cparse_object_get(CParseObject * obj, const char * key)
{
    return cparse_json_get(obj->attributes, key);
}

long long cparse_object_get_number(CParseObject *obj, const char *key)
{
    return cparse_json_get_number(obj->attributes, key);
}

long double cparse_object_get_real(CParseObject *obj, const char *key)
{
    return cparse_json_get_real(obj->attributes, key);
}

bool cparse_object_get_bool(CParseObject *obj, const char *key)
{
    return cparse_json_get_bool(obj->attributes, key);
}

const char *cparse_object_get_string(CParseObject *obj, const char *key)
{
    return cparse_json_get_string(obj->attributes, key);
}

CParseJSONArray *cparse_object_get_array(CParseObject *obj, const char *key)
{
    return cparse_json_get_array(obj->attributes, key);
}

size_t cparse_object_attributes(CParseObject * obj)
{
    return cparse_json_num_keys(obj->attributes);
}


void cparse_object_merge_json(CParseObject *a, CParseJSON *b)
{

    /* objectId, createdAt, and updatedAt are special attributes
     * we're remove them from the b if they exist and add them to a
     */
    CParseJSON *id = cparse_json_remove(b, "objectId");

    if(id != NULL)
        a->objectId = strdup(cparse_json_to_string(id));

    id = cparse_json_remove(b, "createdAt");

    if(id != NULL)
        a->createdAt = cparse_date_time(cparse_json_to_string(id));

    id = cparse_json_remove(b, "updatedAt");

    if(id != NULL)
        a->updatedAt = cparse_date_time(cparse_json_to_string(id));

    cparse_json_copy(a->attributes, b, true);
}

CParseObject* cparse_object_from_json(CParseJSON *jobj)
{
    CParseObject *obj = cparse_object_new();

    cparse_json_copy(obj->attributes, jobj, false);

    return obj;
}

