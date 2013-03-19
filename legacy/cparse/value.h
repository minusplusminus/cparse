
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

/* value initializers */
CParseValue *cparse_value_new();
CParseValue *cparse_value_copy(CParseValue *other);

/* value cleanup */
void cparse_value_free(CParseValue *value);

/* value equality */
/*bool cparse_value_equals(CParseValue *value, CParseValue *other);*/

/* value setters */
/*!
 * sets the variant number value 
 * @param v the value object to set 
 * @param value the number value
 */
CParseValue * cparse_value_new_number(long long);

/*!
 * sets the variant float value @param v the variant to set @param value
 * the float value to set
 */
CParseValue * cparse_value_new_real(double);

CParseValue * cparse_value_new_bool(bool value);

CParseValue * cparse_value_new_string(const char *str);

CParseValue * cparse_value_new_array(CParseArray *value);

/*!
 * gets the set variant type @param v the variant to get the type from
 */
CParseValueType cparse_value_type(CParseValue * t);

/*!
 * gets the variant long integer value @param v the variant to get the
 * value from
 */
long long cparse_value_get_number(CParseValue *);
/*!
 * gets the variant float value @param v the variant to get the value from
 */
double cparse_value_get_real(CParseValue *);

bool cparse_value_get_bool(CParseValue *);

const char *cparse_value_get_string(CParseValue *value);

CParseArray *cparse_value_get_array(CParseValue *value);

/* array initializers */
CParseArray *cparse_array_new();

CParseArray *cparse_array_copy(CParseArray *other);

/* array cleanup */
void cparse_array_free(CParseArray *value);

/* array setters */
void cparse_array_add_number(CParseArray *array, long long value);
void cparse_array_add_real(CParseArray *array, double value);
void cparse_array_add_bool(CParseArray *array, bool value);
void cparse_array_add_string(CParseArray *array, const char *value);
void cparse_array_add_object(CParseArray *array, CParseValue *obj);
void cparse_array_add_array(CParseArray *array, CParseArray *value);

/* array getters */
long long cparse_array_get_number(CParseArray *array, size_t index, long long defaultValue);
double cparse_array_get_real(CParseArray *array, size_t index, double defaultValue);
char *cparse_array_get_string(CParseArray *array, size_t index);
bool cparse_array_get_bool(CParseArray *array, size_t index);
CParseValue *cparse_array_get_object(CParseArray *array, size_t index);
CParseArray *cparse_array_get_array(CParseArray *array, size_t index);

/*CParseValue* cparse_array_remove(CParseArray *value, size_t index);*/

size_t cparse_array_size(CParseArray *value);

/* json stuff */

size_t cparse_value_to_json(CParseValue *value, char *data, size_t maxSize, size_t pos);

size_t cparse_array_to_json(CParseArray *array, char *data, size_t maxSize, size_t pos);

CParseArray* cparse_array_from_json(json_object *obj);

CParseValue *cparse_value_from_json(json_object *jobj);

size_t cparse_value_to_json(CParseValue *value, char *data, size_t pos);

#endif
