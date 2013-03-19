#ifndef CPARSE_DEFINES_H_
#define CPARSE_DEFINES_H_

#include <json/json.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    kCPCachePolicyIgnoreCache = 0,
    kCPCachePolicyCacheOnly,
    kCPCachePolicyNetworkOnly,
    kCPCachePolicyCacheElseNetwork,
    kCPCachePolicyNetworkElseCache,
    kCPCachePolicyCacheThenNetwork
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
    kCPValueNumber = json_type_int,
    kCPValueReal = json_type_double,
    kCPValueString = json_type_string,
    kCPValueBoolean = json_type_boolean,
    kCPValueObject = json_type_object,
    kCPValueArray = json_type_array,
    kCPValueNull = json_type_null
} CParseValueType;

typedef array_list CParseArray;

typedef json_object CParseValue;


#ifdef __cplusplus
}
#endif

#endif
