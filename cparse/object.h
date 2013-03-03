#ifndef CPARSE_OBJECT_H_
#define CPARSE_OBJECT_H_

#include <time.h>
#include <cparse/defines.h>
#include <cparse/acl.h>

struct cparse_object
{
	char *className;
	time_t updatedAt;
	time_t createdAt;
	char *objectId;
	CParseACL *acl;
	CParseTable *attributes;
};

void cparse_object_delete(CParseObject *obj);

CParseObject *cparse_object_with_class_name(const char *className);

CParseObject *cparse_object_with_class_data(const char *className, CParseTable *table);

bool cparse_object_save(CParseObject *obj, CParseError **error);

void cparse_object_set_number(CParseObject *obj, const char *key, long long value);

void cparse_object_set_real(CParseObject *obj, const char *key, long double value);

void cparse_object_set_string(CParseObject *obj, const char *key, const char *value);

void cparse_object_set_object(CParseObject *obj, const char *key, void *value);

#endif
