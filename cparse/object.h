#ifndef CPARSE_OBJECT_H_
#define CPARSE_OBJECT_H_

#include <time.h>
#include <cparse/defines.h>
#include <cparse/acl.h>
#include <json/json.h>

struct cparse_object_attribute
{
    CParseValue* value;
    char *key;
    int flags;
    long hash;
};

struct cparse_object
{
    CParseObjectAttribute *attributes;
    long attr_size, attr_count, attr_pos;
	char *className;
	time_t updatedAt;
	time_t createdAt;
	char *objectId;
	CParseACL *acl;
};

/* initializers */
CParseObject *cparse_object_copy(CParseObject *obj);
CParseObject *cparse_object_with_class_name(const char *className);
CParseObject *cparse_object_with_class_data(const char *className, CParseObject *data);

/* cleanup */
void cparse_object_free(CParseObject *obj);

/* io methods */
bool cparse_object_save(CParseObject *obj, CParseError **error);
void cparse_object_delete(CParseObject *);

/* setters */
void cparse_object_set_value(CParseObject *obj, const char *key, CParseValue *value);
void cparse_object_set_number(CParseObject *obj, const char *key, long long value);
void cparse_object_set_real(CParseObject *obj, const char *key, long double value);
void cparse_object_set_bool(CParseObject *obj, const char *key, bool value);
void cparse_object_set_string(CParseObject *obj, const char *key, const char *value);
void cparse_object_set_object(CParseObject *obj, const char *key, CParseObject *value);
void cparse_object_set_array(CParseObject *obj, const char *key, CParseArray *array);

/* getters */
CParseValue *cparse_object_get_value(CParseObject *, const char *key);
long long cparse_object_get_number(CParseObject *, const char *key, long long);
long double cparse_object_get_real(CParseObject *, const char *key, long double);
bool cparse_object_get_bool(CParseObject *, const char *key);
char *cparse_object_get_string(CParseObject *, const char *key);
CParseObject *cparse_object_get_object(CParseObject *, const char *key);
CParseArray *cparse_object_get_array(CParseObject *, const char *key);

CParseValue* cparse_object_remove_attribute(CParseObject *, const char *key);

/* iterator interface */
void cparse_object_foreach_attribute(CParseObject *, void (*foreach) (CParseValue *));

CParseValue *cparse_object_first_attribute(CParseObject *);
CParseValue *cparse_object_next_attribute(CParseObject *);
bool cparse_object_has_next_attribute(CParseObject *);

long cparse_object_attribute_count(CParseObject *);

void cparse_object_merge_attributes(CParseObject *a, CParseObject *b);

size_t cparse_object_to_json(CParseObject *obj, char *data, size_t pos);

CParseObject *cparse_object_from_json(json_object *jobj);

#endif
