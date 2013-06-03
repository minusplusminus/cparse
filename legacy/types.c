#include <cparse/types.h>
#include <cparse/json.h>
#include "protocol.h"

CParsePointer *cparse_pointer_from_json(CParseJSON *data)
{
    const char *type = cparse_json_get_string(data, KEY_TYPE);

    if(strcmp(type, TYPE_POINTER))
        return NULL;

    CParsePointer *p = malloc(sizeof(CParsePointer));

    p->className = strdup(cparse_json_get_string(data, KEY_CLASS_NAME));

    p->objectId = strdup(cparse_json_get_string(data, KEY_OBJECT_ID));

    return p;
}

CParseJSON *cparse_pointer_to_json(CParsePointer *p)
{
    CParseJSON *data = cparse_json_new();

    cparse_json_set_string(KEY_CLASS_NAME, p->className);

    cparse_json_set_string(KEY_OBJECT_ID, p->objectId);

    cparse_json_set_string(KEY_TYPE, TYPE_POINTER);

    return data;
}

void cparse_pointer_free(CParsePointer *p)
{
    if(p->className)
        free(p->className);

    if(p->objectId)
        free(p->objectId);

    free(p);
}


