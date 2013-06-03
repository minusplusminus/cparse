#ifndef CPARSE_DEFINES_H_
#define CPARSE_DEFINES_H_

#include <json/json.h>
#include <json/json_object_iterator.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char bool;

#undef true
#define true 1
#undef false
#define false 0

typedef enum {
    kCParseCachePolicyIgnoreCache = 0,
    kCParseCachePolicyCacheOnly,
    kCParseCachePolicyNetworkOnly,
    kCParseCachePolicyCacheElseNetwork,
    kCParseCachePolicyNetworkElseCache,
    kCParseCachePolicyCacheThenNetwork
} CParseCachePolicy;

typedef struct cparse_acl CParseACL;

typedef struct cparse_error CParseError;

typedef struct cparse_object CParseObject;

typedef struct cparse_object_attribute CParseObjectAttribute;

typedef struct cparse_query CParseQuery;

typedef struct cparse_user CParseUser;

typedef void (*CParseUserCallback)(CParseUser *user, CParseError *error);

typedef void (*CParseObjectCallback)(CParseObject *obj, CParseError *error);

typedef enum
{
    kCParseJSONNumber = json_type_int,
    kCParseJSONReal = json_type_double,
    kCParseJSONString = json_type_string,
    kCParseJSONBoolean = json_type_boolean,
    kCParseJSONObject = json_type_object,
    kCParseJSONArray = json_type_array,
    kCParseJSONNull = json_type_null
} CParseJSONType;

typedef array_list CParseJSONArray;

typedef json_object CParseJSON;

#ifdef __cplusplus
}
#endif

#endif
