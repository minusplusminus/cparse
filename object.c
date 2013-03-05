#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cparse/object.h>
#include <cparse/error.h>
#include <cparse/value.h>
#include <stdio.h>
#include "io.h"
#include <cparse/util.h>

/*
 * this should be prime
 */
#define ATTRIBUTE_STARTSIZE 1021

#define ACTIVE 1


/* internals */

static unsigned long cparse_object_is_prime(long val)
{
    int i;
    long a, p, exp;

    for (i = 9; i--;)
    {
        a = (rand() % (val - 4)) + 2;
        p = 1;
        exp = val - 1;
        while (exp)
        {
            if (exp & 1)
                p = (p * a) % val;

            a = (a * a) % val;
            exp >>= 1;
        }

        if (p != 1)
            return 0;
    }

    return 1;
}

static unsigned long cparse_object_find_prime_greater_than(unsigned long val)
{
    if (val & 1)
        val += 2;
    else
        val++;

    while (!cparse_object_is_prime(val))
        val += 2;

    return val;
}

static void cparse_object_rehash(CParseObject * obj)
{
    long size = obj->attr_size;
    CParseObjectAttribute *attr = obj->attributes;

    obj->attr_size = cparse_object_find_prime_greater_than(size << 1);
    obj->attributes = (CParseObjectAttribute *) calloc(sizeof(CParseObjectAttribute), obj->attr_size);
    obj->attr_count = 0;

    while (--size >= 0)
        if (attr[size].flags == ACTIVE)
            cparse_object_set_value(obj, attr[size].key, attr[size].value);

    free(attr);
}

/*
 * Returns a hash code for the provided string.
 */
static unsigned long cparse_object_hash_key(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
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

    obj->attr_size = ATTRIBUTE_STARTSIZE;
    obj->attributes = (CParseObjectAttribute *) calloc(sizeof(CParseObjectAttribute), obj->attr_size);
    obj->attr_count = 0;
    obj->attr_pos = 0;

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

    cparse_object_merge_attributes(obj, other);

    return obj;
}

CParseObject *cparse_object_with_class_name(const char *className)
{
	CParseObject *obj = cparse_object_new();

	obj->className = strdup(className);

	return obj;
}

CParseObject *cparse_object_with_class_data(const char *className, CParseObject* attributes)
{
	CParseObject *obj = cparse_object_with_class_name(className);

	cparse_object_merge_attributes(obj, attributes);

	return obj;
}

/* cleanup */
void cparse_object_free(CParseObject *obj) {

    if (obj->attr_count)
    {
        for (int i = 0; i < obj->attr_size; i++)
        {
            if (obj->attributes[i].value)
                cparse_value_free(obj->attributes[i].value);
        }
    }
    if(obj->attributes)
        free(obj->attributes);

	if(obj->className)
		free(obj->className);
	if(obj->objectId)
		free(obj->objectId);
	free(obj);
}

/* io */

void cparse_object_refresh(CParseObject *obj, CParseError **error)
{
    CParseRequest *request;
    CParseObject *response;
    char buf[BUFSIZ+1];

    if(!obj->objectId || !*obj->objectId) {
        return;
    }

    request = cparse_request_new();

    /* build the request */
    snprintf(buf, BUFSIZ, "classes/%s/%s", obj->className, obj->objectId);

    request->path = strdup(buf);

    request->method = kCPRequestGet;

    /* do the deed */
    response = cparse_io_request_json(request, error);

    if(error != NULL && *error != NULL)
    {
        return;
    }

    /* merge the response with the object */
    cparse_object_merge_attributes(obj, response);
}

bool cparse_object_save(CParseObject *obj, CParseError **error)
{
	CParseObject *response;
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
	request->payload = calloc(sizeof(char), MAX_PAYLOAD_SIZE);

	request->payload_size = cparse_object_to_json(obj, request->payload, 0);

    /* do the deed */
	response = cparse_io_request_json(request, error);

	if(error != NULL && *error != NULL) {
		return false;
	}

    /* merge the result with the object */
    cparse_object_merge_attributes(obj, response);

	return true;
}

/* setters */

void cparse_object_set_number(CParseObject *obj, const char *key, long long value)
{
	assert(obj != NULL);
	assert(obj->attributes != NULL);

	CParseValue v;
	cparse_value_set_number(&v, value);

	cparse_object_set_value(obj, key, &v);
}

void cparse_object_set_real(CParseObject *obj, const char *key, long double value)
{
	assert(obj != NULL);
	assert(obj->attributes != NULL);

	CParseValue v;
	cparse_value_set_real(&v, value);

	cparse_object_set_value(obj, key, &v);
}
void cparse_object_set_bool(CParseObject *obj, const char *key, bool value)
{
    assert(obj != NULL);
    assert(obj->attributes != NULL);

    CParseValue v;
    cparse_value_set_bool(&v, value);

    cparse_object_set_value(obj, key, &v);
}

void cparse_object_set_string(CParseObject *obj, const char *key, const char *value)
{
	assert(obj != NULL);
	assert(obj->attributes != NULL);

	CParseValue v;
	cparse_value_set_string(&v, value);

	cparse_object_set_value(obj, key, &v);
}

void cparse_object_set_object(CParseObject *obj, const char *key, CParseObject *value)
{
	assert(obj != NULL);
	assert(obj->attributes != NULL);

	CParseValue v;
	cparse_value_set_object(&v, value);

	cparse_object_set_value(obj, key, &v);
}


void cparse_object_foreach_attribute(CParseObject * obj, void (*foreach) (CParseValue *data))
{
    if (obj->attr_count)
    {
        for (int i = 0; i < obj->attr_size; i++)
        {
            if (obj->attributes[i].value)            
                (foreach) (obj->attributes[i].value);
        }
    }
}

CParseValue *cparse_object_first_attribute(CParseObject * obj)
{
    obj->attr_pos = 0;

    if (obj->attr_pos < obj->attr_size)
    {
        return obj->attributes[obj->attr_pos].value;
    }
    return NULL;
}

CParseValue *cparse_object_next_attribute(CParseObject * obj)
{
    if (++obj->attr_pos < obj->attr_size)
        return obj->attributes[obj->attr_pos].value;

    return NULL;
}

bool cparse_object_has_next_attribute(CParseObject * obj)
{
    return obj->attr_pos < obj->attr_size && obj->attributes[obj->attr_pos].value;
}


void cparse_object_set_value(CParseObject * obj, const char* key, CParseValue *data)
{
    long index, i, step, hash;

    if (obj->attr_size <= obj->attr_count)
        cparse_object_rehash(obj);


    hash = cparse_object_hash_key(key);
        
    do
    {
        index = hash % obj->attr_size;
        step = (hash % (obj->attr_size - 2)) + 1;

        for (i = 0; i < obj->attr_size; i++)
        {
            if (obj->attributes[index].flags & ACTIVE)
            {
                if (obj->attributes[index].hash == hash)
                {
                    obj->attributes[index].value = cparse_value_copy(data);
                    return;
                }
            }
            else
            {
                obj->attributes[index].flags |= ACTIVE;
                obj->attributes[index].value = cparse_value_copy(data);
                obj->attributes[index].hash = hash;
                obj->attributes[index].key = strdup(key);
                ++obj->attr_count;
                return;
            }

            index = (index + step) % obj->attr_size;
        }

        /*
         * it should not be possible that we EVER come this far, but
         * unfortunately not every generated prime number is prime
         * (Carmichael numbers...)
         */
        cparse_object_rehash(obj);
    }
    while (1);
}

CParseValue *cparse_object_remove_attribute(CParseObject * obj, const char* key)
{
    long index, i, step, hash;

    hash = cparse_object_hash_key(key);

    index = hash % obj->attr_size;
    step = (hash % (obj->attr_size - 2)) + 1;

    for (i = 0; i < obj->attr_size; i++)
    {
        if (obj->attributes[index].value)
        {
            if (obj->attributes[index].hash == hash)
            {
                if (obj->attributes[index].flags & ACTIVE)
                {
                    obj->attributes[index].flags &= ~ACTIVE;
                    --obj->attr_count;
                    return obj->attributes[index].value;
                }
                else	/* in, but not active (i.e. deleted) */
                    return NULL;
            }
        }
        else		/* found an empty place (can't be in) */
            return NULL;

        index = (index + step) % obj->attr_size;
    }
    /*
     * everything searched through, but not in
     */
    return NULL;
}

/* getters */

CParseValue *cparse_object_get_value(CParseObject * obj, const char * key)
{
    if (obj->attr_count)
    {
        long index, i, step, hash;
        hash = cparse_object_hash_key(key);
        index = hash % obj->attr_size;
        step = (hash % (obj->attr_size - 2)) + 1;

        for (i = 0; i < obj->attr_size; i++)
        {
            if (obj->attributes[index].hash == hash)
            {
                if (obj->attributes[index].flags & ACTIVE)
                    return obj->attributes[index].value;
                break;
            }
            else if (!obj->attributes[index].value)
                break;

            index = (index + step) % obj->attr_size;
        }
    }
    return NULL;
}

long long cparse_object_get_number(CParseObject *obj, const char *key, long long defaultValue)
{
    CParseValue *value = cparse_object_get_value(obj, key);

    if(value)
        return cparse_value_get_number(value);
    else
        return defaultValue;
}

long double cparse_object_get_real(CParseObject *obj, const char *key, long double defaultValue)
{
    CParseValue *value = cparse_object_get_value(obj, key);

    if(value)
        return cparse_value_get_real(value);
    else
        return defaultValue;
}

bool cparse_object_get_bool(CParseObject *obj, const char *key)
{
    CParseValue *value = cparse_object_get_value(obj, key);

    if(value)
        return cparse_value_get_bool(value);
    else
        return false;
}

char *cparse_object_get_string(CParseObject *obj, const char *key)
{
    CParseValue *value = cparse_object_get_value(obj, key);

    if(value)
        return cparse_value_get_string(value);
    else
        return NULL;
}

CParseObject *cparse_object_get_object(CParseObject *obj, const char *key)
{
    CParseValue *value = cparse_object_get_value(obj, key);

    if(value)
        return cparse_value_get_object(value);
    else
        return NULL;
}

CParseArray *cparse_object_get_array(CParseObject *obj, const char *key)
{
    CParseValue *value = cparse_object_get_value(obj, key);

    if(value)
        return cparse_value_get_array(value);
    else
        return NULL;
}

long cparse_object_attribute_count(CParseObject * obj)
{
    return obj->attr_count;
}


void cparse_object_merge_attributes(CParseObject *a, CParseObject *b)
{

    CParseValue *id = cparse_object_remove_attribute(b, "objectId");

    if(id != NULL)
        a->objectId = strdup(cparse_value_get_string(id));

    id = cparse_object_remove_attribute(b, "createdAt");

    if(id != NULL)
        a->createdAt = cparse_date_time(cparse_value_get_string(id));

    id = cparse_object_remove_attribute(b, "updatedAt");

    if(id != NULL)
        a->updatedAt = cparse_date_time(cparse_value_get_string(id));

    if(b->attr_count)
    {
        int i;

        for(i = 0; i < b->attr_size; i++)
        {
            if(b->attributes[i].value)
                cparse_object_set_value(a, b->attributes[i].key, b->attributes[i].value);
        }
    }
}

CParseObject* cparse_object_from_json(json_object *jobj)
{
    CParseObject *obj = cparse_object_new();

    json_object_object_foreach(jobj, key, val)
    {
        CParseValue *value = cparse_value_from_json(val);

        if(value != NULL) {
            cparse_object_set_value(obj, key, value);
        }
    }
    return obj;
}

static size_t cparse_object_attribute_to_json(CParseObjectAttribute *attr, char *data, size_t pos)
{
    pos += sprintf(&data[pos], "\"%s\":", attr->key);

    pos = cparse_value_to_json(attr->value, data, pos);

    return pos;
}

size_t cparse_object_to_json(CParseObject *obj, char *data, size_t pos)
{
    assert(obj != NULL);

    if(obj->attr_count)
    {
        int i, c;

        pos += sprintf(&data[pos], "{");

        for(c = i = 0; i < obj->attr_size; i++)
        {
            if(obj->attributes[i].value)
            {
                pos = cparse_object_attribute_to_json(&obj->attributes[i], data, pos);

                if(++c < obj->attr_count) {
                    pos += sprintf(&data[pos], ",");
                }
            }

        }

        pos += sprintf(&data[pos], "}");
    }
    return pos;
}
