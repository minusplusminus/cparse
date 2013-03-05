#ifndef CPARSE_DEFINES_H_
#define CPARSE_DEFINES_H_

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
    kCPValueNumber,
    kCPValueReal,
    kCPValueString,
    kCPValueBoolean,
    kCPValueObject,
    kCPValueArray
} CParseValueType;

typedef struct cparse_array CParseArray;

typedef struct cparse_value CParseValue;

#endif
