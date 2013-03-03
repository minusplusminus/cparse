#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cparse/object.h>
#include <cparse/table.h>
#include <cparse/error.h>
#include <stdio.h>
#include "io.h"

static CParseObject *cparse_object_new()
{
	CParseObject *obj = malloc(sizeof(CParseObject));

	obj->className = NULL;
	obj->objectId = NULL;
	obj->acl = NULL;
	obj->createdAt = 0;
	obj->updatedAt = 0;
	obj->attributes = cparse_table_new(0);

	return obj;
}

CParseObject *cparse_object_with_class_name(const char *className)
{
	CParseObject *obj = cparse_object_new();

	obj->className = strdup(className);

	return obj;
}

CParseObject *cparse_object_with_class_data(const char *className, CParseTable* attributes)
{
	CParseObject *obj = cparse_object_new();

	obj->attributes = attributes;

	return obj;
}

void cparse_object_delete(CParseObject *obj) {
	if(obj->attributes)
		cparse_table_delete(obj->attributes);
	if(obj->className)
		free(obj->className);
	if(obj->objectId)
		free(obj->objectId);
	free(obj);
}

bool cparse_object_save(CParseObject *obj, CParseError **error)
{
	CParseTable *response;
	CParseRequest *request = cparse_request_new();
	char buf[BUFSIZ+1];

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

	request->payload = calloc(sizeof(char), MAX_PAYLOAD_SIZE);

	request->payload_size = cparse_table_to_json(obj->attributes, request->payload, 0);

	response = cparse_io_request_json(request, error);

	if(error != NULL && *error != NULL) {
		fputs((*error)->message, stderr);
		return false;
	}
	else {
		CParseValue *id = cparse_table_remove(response, "objectId");
		if(id != NULL)
			obj->objectId = strdup(cparse_value_get_string(id));

		cparse_table_merge(obj->attributes, response);

		return true;
	}
}


void cparse_object_set_number(CParseObject *obj, const char *key, long long value)
{
	assert(obj != NULL);
	assert(obj->attributes != NULL);

	CParseValue *v = cparse_value_new();
	cparse_value_set_number(v, value);

	cparse_table_insert(obj->attributes, v, key);
}

void cparse_object_set_real(CParseObject *obj, const char *key, long double value)
{
	assert(obj != NULL);
	assert(obj->attributes != NULL);

	CParseValue *v = cparse_value_new();
	cparse_value_set_real(v, value);

	cparse_table_insert(obj->attributes, v, key);
}

void cparse_object_set_string(CParseObject *obj, const char *key, const char *value)
{
	assert(obj != NULL);
	assert(obj->attributes != NULL);

	CParseValue *v = cparse_value_new();
	cparse_value_set_string(v, value);

	cparse_table_insert(obj->attributes, v, key);
}

void cparse_object_set_object(CParseObject *obj, const char *key, void *value)
{
	assert(obj != NULL);
	assert(obj->attributes != NULL);

	CParseValue *v = cparse_value_new();
	cparse_value_set_object(v, value);

	cparse_table_insert(obj->attributes, v, key);
}