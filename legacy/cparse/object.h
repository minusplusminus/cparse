#ifndef CPARSE_OBJECT_H_
#define CPARSE_OBJECT_H_

#include <time.h>
#include <cparse/defines.h>
#include <cparse/acl.h>
#include <json/json.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct cparse_object
    {
        CParseJSON *attributes;
        char *className;
        time_t updatedAt;
        time_t createdAt;
        char *objectId;
        CParseACL *acl;
    };

    /* initializers */
    CParseObject *cparse_object_copy(CParseObject *obj);
    CParseObject *cparse_object_with_class_name(const char *className);
    CParseObject *cparse_object_with_class_data(const char *className, CParseJSON *data);

    /* cleanup */
    void cparse_object_free(CParseObject *obj);

    /* client/rest methods */
    bool cparse_object_save(CParseObject *obj, CParseError **error);
    pthread_t cparse_object_save_in_background(CParseObject *obj, CParseObjectCallback callback);

    bool cparse_object_delete(CParseObject *, CParseError **error);
    pthread_t cparse_object_delete_in_background(CParseObject *obj, CParseObjectCallback callback);

    bool cparse_object_refresh(CParseObject *, CParseError **error);
    pthread_t cparse_object_refresh_in_background(CParseObject *obj, CParseObjectCallback callback);

    bool cparse_object_fetch(CParseObject *, CParseError **error);
    pthread_t cparse_object_fetch_in_background(CParseObject *obj, CParseObjectCallback callback);

    /* setters */
    void cparse_object_set_number(CParseObject *obj, const char *key, long long value);
    void cparse_object_set_real(CParseObject *obj, const char *key, double value);
    void cparse_object_set_bool(CParseObject *obj, const char *key, bool value);
    void cparse_object_set_string(CParseObject *obj, const char *key, const char *value);
    void cparse_object_set(CParseObject *obj, const char *key, CParseJSON *value);

    void cparse_object_set_reference(CParseObject *obj, const char *key, CParseObject *ref);

    /* getters */
    CParseJSON *cparse_object_get(CParseObject *, const char *key);
    long long cparse_object_get_number(CParseObject *, const char *key);
    long double cparse_object_get_real(CParseObject *, const char *key);
    bool cparse_object_get_bool(CParseObject *, const char *key);
    const char *cparse_object_get_string(CParseObject *, const char *key);
    CParseJSONArray *cparse_object_get_array(CParseObject *, const char *key);

    CParseJSON* cparse_object_remove(CParseObject *, const char *key);
    bool cparse_object_contains(CParseObject *obj, const char *key);

    /* iterator interface */
    void cparse_object_foreach(CParseObject *, void (*foreach) (CParseJSON *));

    size_t cparse_object_attributes(CParseObject *);

    void cparse_object_merge_json(CParseObject *a, CParseJSON *b);

    CParseObject *cparse_object_from_json(CParseJSON *obj);

#ifdef __cplusplus
}
#endif

#endif
