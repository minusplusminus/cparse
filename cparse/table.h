
#ifndef CPARSE_TABLE_H_
#define CPARSE_TABLE_H_

#include <stdbool.h>
#include <cparse/defines.h>
#include <cparse/value.h>
#include <json/json.h>

struct cparse_table_entry
{
    CParseValue* value;
    char *key;
    int flags;
    long hash;
};

struct cparse_table
{
    CParseTableEntry *entry;
    long size, count, pos;
};

CParseTable *cparse_table_new(long startsize);
CParseTable *cparse_table_copy(CParseTable *orig);

/** Inserts a new element into the table. */
void cparse_table_insert(CParseTable *, CParseValue *data, const char *key);

/** Removes the storage for the element of the key and returns the element. */
CParseValue *cparse_table_remove(CParseTable *, const char *key);

/** Returns the element for the key. */
CParseValue *cparse_table_get(CParseTable *, const char *key);

void cparse_table_foreach(CParseTable *, void (*foreach) (CParseValue *));

CParseValue *cparse_table_start(CParseTable *);
CParseValue *cparse_table_next(CParseTable *);
bool cparse_table_has_next(CParseTable *);

long cparse_table_count(CParseTable *);

void cparse_table_delete(CParseTable *);

void cparse_table_merge(CParseTable *a, CParseTable *b);

size_t cparse_table_to_json(CParseTable *table, char *data, size_t pos);

CParseTable *cparse_table_from_json(json_object *jobj);

#endif
