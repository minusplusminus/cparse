
/*!
 * @header Structure, methods and constants related to variants
 * @ignore __VARIANT_H_
 */
#ifndef CPARSE_VALUE_H_
#define CPARSE_VALUE_H_

#include <stdlib.h>
#include <cparse/defines.h>
#include <json/json.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* value initializers */
    CParseJSON *cparse_json_new();
    CParseJSON *cparse_json_new_reference(CParseJSON *orig);
    CParseJSON * cparse_json_new_number(long long);
    CParseJSON * cparse_json_new_real(double);
    CParseJSON * cparse_json_new_bool(bool value);
    CParseJSON * cparse_json_new_string(const char *str);
    CParseJSON *cparse_json_new_array();

    /* copy method */
    void cparse_json_copy(CParseJSON *orig, CParseJSON *other, bool replaceOnConflict);

    /* value cleanup */
    void cparse_json_free(CParseJSON *value);

    /* value setters */

    void cparse_json_set(CParseJSON *obj, const char *key, CParseJSON *value);
    void cparse_json_set_number(CParseJSON *obj, const char *key, long long value);
    void cparse_json_set_real(CParseJSON *obj, const char *key, double value);
    void cparse_json_set_string(CParseJSON *obj, const char *key, const char* value);
    void cparse_json_set_bool(CParseJSON *obj, const char *key, bool value);

    /* object getters */
    CParseJSON *cparse_json_get(CParseJSON *obj, const char *key);
    long long cparse_json_get_number(CParseJSON *obj, const char *key);
    double cparse_json_get_real(CParseJSON *obj, const char *key);
    bool cparse_json_get_bool(CParseJSON *obj, const char *key);
    const char *cparse_json_get_string(CParseJSON *obj, const char *key);
    CParseJSONArray *cparse_json_get_array(CParseJSON *obj, const char *key);

    json_type cparse_json_type(CParseJSON *obj);
    int cparse_json_num_keys(CParseJSON *obj);
    bool cparse_json_contains(CParseJSON *obj, const char *key);

    CParseJSON* cparse_json_remove(CParseJSON *obj, const char *key);

    /* array setters */
    void cparse_json_array_add_number(CParseJSON *array, long long value);
    void cparse_json_array_add_real(CParseJSON *array, double value);
    void cparse_json_array_add_bool(CParseJSON *array, bool value);
    void cparse_json_array_add_string(CParseJSON *array, const char *value);
    void cparse_json_array_add(CParseJSON *array, CParseJSON *obj);

    /* array getters */
    long long cparse_json_array_get_number(CParseJSON *array, size_t index);
    double cparse_json_array_get_real(CParseJSON *array, size_t index);
    const char *cparse_json_array_get_string(CParseJSON *array, size_t index);
    bool cparse_json_array_get_bool(CParseJSON *array, size_t index);
    CParseJSON *cparse_json_array_get(CParseJSON *array, size_t index);
    CParseJSONArray *cparse_json_array_get_array(CParseJSON *array, size_t index);

    size_t cparse_json_array_size(CParseJSON *value);

    /* converters */

    long long cparse_json_to_number(CParseJSON *);
    double cparse_json_to_real(CParseJSON *);
    bool cparse_json_to_bool(CParseJSON *);
    const char *cparse_json_to_string(CParseJSON *value);
    CParseJSONArray *cparse_json_to_array(CParseJSON *value);

    const char* cparse_json_to_json_string(CParseJSON *value);

#ifdef __cplusplus
}
#endif

#endif
