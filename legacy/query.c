#include <stdlib.h>
#include <string.h>
#include <cparse/query.h>

void cparse_query_clear_all_caches()
{

}

CParseQuery *cparse_query_new()
{
    CParseQuery *query = malloc(sizeof(CParseQuery));

    query->cachePolicy = kCParseCachePolicyIgnoreCache;
    query->className = NULL;
    query->limit = 0;
    query->skip = 0;
    query->trace = false;

    return query;
}

void cparse_query_delete(CParseQuery *query)
{
    if(query->className)
        free(query->className);

    free(query);
}

CParseObject *cparse_query_get_object_of_class(const char *objectClass, const char *objectId, CParseError **error)
{

    return NULL;
}

CParseObject *cparse_query_get_object_with_id(const char *objectId, CParseError **error)
{
    return NULL;
}

CParseQuery *cparse_query_or_with_subqueries(CParseQuery **subqueries, size_t count)
{
    return NULL;
}

CParseQuery *cparse_query_with_class_name(const char *className)
{
    CParseQuery *query = cparse_query_new();

    query->className = strdup(className);

    return query;
}


void cparse_query_cancel(CParseQuery *query)
{

}

void cparse_query_clear_cache(CParseQuery *query)
{

}

int cparse_query_count_objects(CParseQuery *query, CParseError **error)
{
    return 0;
}
