
/*!
 * @header Structure, methods and constants related to variants 
 * @ignore __VARIANT_H_
 */
#ifndef CPARSE_VALUE_H_
#define CPARSE_VALUE_H_

#include <stdlib.h>
#include <stdbool.h>
#include <cparse/defines.h>
#include <json/json.h>

/*!
 * @typedef variant_t @abstract Represents a variable that can have
 * multiple types @field type the type of variable @field char_value the
 * character value @field short_value the short integer value @field
 * int_value the integer value @field long_value the long integer value
 * @field float_value the float_value @field double_value the double value
 * @field ptr_value the generic pointer value
 */
struct cparse_value
{
    int type;
    union
    {
        long long number_value;
        long double real_value;
        void *object_value;
    };
};

struct cparse_array
{
    CParseValue **value;
    size_t size;
};

/* value initializers */
CParseValue *cparse_value_new();
CParseValue *cparse_value_copy(CParseValue *other);

/* value cleanup */
void cparse_value_free(CParseValue *value);

/* value equality */
bool cparse_value_equals(CParseValue *value, CParseValue *other);

/* value setters */
/*!
 * sets the variant number value 
 * @param v the value object to set 
 * @param value the number value
 */
void cparse_value_set_number(CParseValue *value, long long);

/*!
 * sets the variant float value @param v the variant to set @param value
 * the float value to set
 */
void cparse_value_set_real(CParseValue *value, long double);

void cparse_value_set_bool(CParseValue *v, bool value);

void cparse_value_set_string(CParseValue *value, const char *str);

/*!
 * sets the variant pointer value @param v the variant to set @param value
 * the pointer value to set
 */
void cparse_value_set_object(CParseValue *, CParseObject *);

void cparse_value_set_array(CParseValue *v, CParseArray *value);

/*!
 * gets the set variant type @param v the variant to get the type from
 */
int cparse_value_type(CParseValue * t);

/*!
 * gets the variant long integer value @param v the variant to get the
 * value from
 */
long long cparse_value_get_number(CParseValue *);
/*!
 * gets the variant float value @param v the variant to get the value from
 */
long double cparse_value_get_real(CParseValue *);

bool cparse_value_get_bool(CParseValue *);

char *cparse_value_get_string(CParseValue *value);

CParseArray *cparse_value_get_array(CParseValue *value);

/*!
 * gets the variant pointer value @param v the variant to get the value
 * from
 */
CParseObject *cparse_value_get_object(CParseValue *);


/* array initializers */
CParseArray *cparse_array_new();
CParseArray *cparse_array_copy(CParseArray *other);

/* array cleanup */
void cparse_array_free(CParseArray *value);

/* array setters */
void cparse_array_add_value(CParseArray *array, CParseValue *value);
void cparse_array_add_number(CParseArray *array, long long value);
void cparse_array_add_real(CParseArray *array, long double value);
void cparse_array_add_bool(CParseArray *array, bool value);
void cparse_array_add_string(CParseArray *array, const char *value);
void cparse_array_add_object(CParseArray *array, CParseObject *obj);
void cparse_array_add_array(CParseArray *array, CParseArray *value);

/* array getters */
CParseValue *cparse_array_get_value(CParseArray *array, size_t index);
long long cparse_array_get_number(CParseArray *array, size_t index, long long defaultValue);
long double cparse_array_get_real(CParseArray *array, size_t index, long double defaultValue);
char *cparse_array_get_string(CParseArray *array, size_t index);
bool cparse_array_get_bool(CParseArray *array, size_t index);
CParseObject *cparse_array_get_object(CParseArray *array, size_t index);
CParseArray *cparse_array_get_array(CParseArray *array, size_t index);

CParseValue* cparse_array_remove(CParseArray *value, size_t index);

size_t cparse_array_size(CParseArray *value);

/* json stuff */

size_t cparse_value_to_json(CParseValue *value, char *data, size_t pos);

size_t cparse_array_to_json(CParseArray *array, char *data, size_t pos);

CParseArray* cparse_array_from_json(json_object *obj);

CParseValue *cparse_value_from_json(json_object *jobj);

size_t cparse_value_to_json(CParseValue *value, char *data, size_t pos);

#endif
