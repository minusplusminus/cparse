#ifndef CPARSE_QUERY_H_
#define CPARSE_QUERY_H_

#include <stdbool.h>
#include <cparse/defines.h>
#include <cparse/error.h>
#include <cparse/object.h>

struct cparse_query
{
	CParseCachePolicy cachePolicy;
	char *className;
	int limit;
	int skip;
	bool trace;
};

void cparse_query_clear_all_caches();

CParseObject *cparse_query_get_object_of_class(const char *objectClass, const char *objectId, CParseError **error);

CParseObject *cparse_query_get_object_with_id(const char *objectId, CParseError **error);

CParseQuery *cparse_query_or_with_subqueries(CParseQuery **subqueries, size_t count);

CParseQuery *cparse_query_with_class_name(const char *className);

void cparse_query_cancel(CParseQuery *query);

void cparse_query_clear_cache(CParseQuery *query);

int cparse_query_count_objects(CParseQuery *query, CParseError **error);

#endif
