#include <string.h>
#include <stdio.h>
#include <cparse/json.h>
#include <cparse/object.h>

#include <json/json_object_private.h>
#include <json/json_object_iterator.h>

#include <assert.h>

/*initializers */
CParseJSON *cparse_json_new()
{
    return json_object_new_object();
}

CParseJSON *cparse_json_new_reference(CParseJSON *orig)
{
    return json_object_get(orig);
}

CParseJSON *cparse_json_new_number(long long value)
{
    return json_object_new_int64(value);
}

CParseJSON *cparse_json_new_bool(bool value)
{
    return json_object_new_boolean(value);
}

CParseJSON *cparse_json_new_real(double value)
{
    return json_object_new_double(value);
}

CParseJSON *cparse_json_new_string(const char *value)
{
    return json_object_new_string(value);
}

void cparse_json_copy(CParseJSON *orig, CParseJSON *other, bool replaceOnConflict)
{
    json_object_object_foreach(other, key, val)
    {
        if(replaceOnConflict && json_object_object_get_ex(orig, key, NULL))
        {
            json_object_object_del(orig, key);
        }

        json_object_object_add(orig, key, json_object_get(val));
    }
}

CParseJSON *cparse_json_new_array()
{
    return json_object_new_array();
}

/* destructors */
void cparse_json_free(CParseJSON *value)
{
    json_object_put(value);
}

/* setters */

void cparse_json_set_number(CParseJSON *obj, const char *key, long long value)
{
    json_object_object_add(obj, key, json_object_new_int64(value));
}

void cparse_json_set_real(CParseJSON *obj, const char *key, double value)
{
    json_object_object_add(obj, key, json_object_new_double(value));
}

void cparse_json_set_bool(CParseJSON *obj, const char *key, bool value)
{
    json_object_object_add(obj, key, json_object_new_boolean(value));
}

void cparse_json_set_string(CParseJSON *obj, const char *key, const char *value)
{
    json_object_object_add(obj, key, json_object_new_string(value));
}

void cparse_json_set(CParseJSON *obj, const char *key, CParseJSON *value)
{
    json_object_object_add(obj, key, value);
}

/* getters */

CParseJSON *cparse_json_get(CParseJSON *obj, const char *key)
{
    return json_object_object_get(obj, key);
}
long long cparse_json_get_number(CParseJSON *obj, const char *key)
{
    return json_object_get_int64(json_object_object_get(obj, key));
}
double cparse_json_get_real(CParseJSON *obj, const char *key)
{
    return json_object_get_double(json_object_object_get(obj, key));
}

bool cparse_json_get_bool(CParseJSON *obj, const char *key)
{
    return json_object_get_boolean(json_object_object_get(obj, key));
}
const char *cparse_json_get_string(CParseJSON *obj, const char *key)
{
    return json_object_get_string(json_object_object_get(obj, key));
}

CParseJSONArray *cparse_json_get_array(CParseJSON *obj, const char *key)
{
    return json_object_get_array(json_object_object_get(obj, key));
}

/* array setters */
void cparse_json_array_add_number(CParseJSON *arr, long long value)
{
    json_object_array_add(arr, json_object_new_int64(value));
}

void cparse_json_array_add_real(CParseJSON *arr, double real)
{
    json_object_array_add(arr, json_object_new_double(real));
}

void cparse_json_array_add_bool(CParseJSON *arr, bool b)
{
    json_object_array_add(arr, json_object_new_boolean(b));
}


void cparse_json_array_add_string(CParseJSON *arr, const char *value)
{
    json_object_array_add(arr, json_object_new_string(value));
}


void cparse_json_array_add(CParseJSON *arr, CParseJSON *value)
{
    json_object_array_add(arr, value);
}

/* array getters */
long long cparse_json_array_get_number(CParseJSON *arr, size_t index)
{
    CParseJSON *value = json_object_array_get_idx(arr, index);

    return json_object_get_int64(value);
}
bool cparse_json_array_get_bool(CParseJSON *arr, size_t index)
{
    CParseJSON *value = json_object_array_get_idx(arr, index);

    return json_object_get_boolean(value);
}

double cparse_json_array_get_real(CParseJSON *arr, size_t index)
{
    CParseJSON *value = json_object_array_get_idx(arr, index);

    return json_object_get_double(value);
}

const char* cparse_json_array_get_string(CParseJSON *arr, size_t index)
{
    CParseJSON *value = json_object_array_get_idx(arr, index);

    return json_object_get_string(value);
}

CParseJSON* cparse_json_array_get(CParseJSON *arr, size_t index)
{
    return json_object_array_get_idx(arr, index);
}

CParseJSONArray* cparse_json_array_get_array(CParseJSON *arr, size_t index)
{
    CParseJSON *value = json_object_array_get_idx(arr, index);

    return json_object_get_array(value);
}

size_t cparse_json_array_size(CParseJSON *array)
{
    return json_object_array_length(array);
}

int cparse_json_num_keys(CParseJSON *obj)
{
    return json_object_get_object (obj)->count;
}

CParseJSON *cparse_json_remove(CParseJSON *obj, const char *key)
{
    CParseJSON *orig = json_object_object_get(obj, key);

    if(orig != NULL)
    {
        orig = json_object_get(orig);
        json_object_object_del(obj, key);
    }

    return orig;
}

/* converters */


long long cparse_json_to_number(CParseJSON *v)
{
    return json_object_get_int(v);
}

bool cparse_json_to_bool(CParseJSON *v)
{
    return json_object_get_boolean(v);
}

double cparse_json_to_real(CParseJSON *v)
{
    return json_object_get_double(v);
}

const char *cparse_json_to_string(CParseJSON *v)
{
    return json_object_get_string(v);
}

json_type cparse_json_type(CParseJSON *v)
{
    return json_object_get_type(v);
}

bool cparse_json_contains(CParseJSON *obj, const char *key)
{
    return json_object_object_get_ex(obj, key, NULL);
}

CParseJSONArray *cparse_json_to_array(CParseJSON *v)
{
    return json_object_get_array(v);
}

const char* cparse_json_to_json_string(CParseJSON *obj)
{
    return json_object_to_json_string_ext(obj, JSON_C_TO_STRING_PLAIN);
}

