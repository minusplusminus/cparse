
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <cparse/table.h>
#include <json/json.h>
#include <cparse/value.h>


/*
 * this should be prime
 */
#define TABLE_STARTSIZE 1021

#define ACTIVE 1

static unsigned long cparse_table_is_prime(long val)
{
    int i;
    long a, p, exp;

    for (i = 9; i--;)
    {
        a = (rand() % (val - 4)) + 2;
        p = 1;
        exp = val - 1;
        while (exp)
        {
            if (exp & 1)
                p = (p * a) % val;

            a = (a * a) % val;
            exp >>= 1;
        }

        if (p != 1)
            return 0;
    }

    return 1;
}

static unsigned long cparse_table_find_prime_greater_than(unsigned long val)
{
    if (val & 1)
        val += 2;
    else
        val++;

    while (!cparse_table_is_prime(val))
        val += 2;

    return val;
}

static void cparse_table_rehash(CParseTable * table)
{
    long size = table->size;
    CParseTableEntry *entry = table->entry;

    table->size = cparse_table_find_prime_greater_than(size << 1);
    table->entry = (CParseTableEntry *) calloc(sizeof(CParseTableEntry), table->size);
    table->count = 0;

    while (--size >= 0)
        if (entry[size].flags == ACTIVE)
            cparse_table_insert(table, entry[size].value, entry[size].key);

    free(table);
}

/*
 * Returns a hash code for the provided string.
 */
static unsigned long cparse_table_hash_key(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}


CParseTable *cparse_table_new(long startsize)
{
    CParseTable *table = (CParseTable *) malloc(sizeof(CParseTable));

    if (!startsize)
        startsize = TABLE_STARTSIZE;
    else
        startsize = cparse_table_find_prime_greater_than(startsize - 2);

    table->entry = (CParseTableEntry *) calloc(sizeof(CParseTableEntry), startsize);
    table->size = startsize;
    table->count = 0;

    return table;
}

CParseTable *cparse_table_copy(CParseTable *other)
{
    CParseTable *table = cparse_table_new(other->size);

    if(other->count)
    {
        table->count = other->count;
    
        for(int i = 0; i < other->size; i++)
        {
            if(other->entry[i].value)
            {
                table->entry[i].value = cparse_value_copy(other->entry[i].value);
            }
        }
    }
    return table;
}
void cparse_table_foreach(CParseTable * table, void (*foreach) (CParseValue *data))
{
    if (table->count)
    {

        for (int i = 0; i < table->size; i++)
        {
            if (table->entry[i].value)            
                (foreach) (table->entry[i].value);
        }
    }
}

CParseValue *cparse_table_start(CParseTable * table)
{
    table->pos = 0;

    if (table->pos < table->size)
    {
        return table->entry[table->pos].value;
    }
    return 0;
}

CParseValue *cparse_table_next(CParseTable * table)
{
    if (++table->pos < table->size)
        return table->entry[table->pos].value;

    return 0;
}

bool cparse_table_has_next(CParseTable * table)
{
    return table->pos < table->size && table->entry[table->pos].value;
}


void cparse_table_insert(CParseTable * table, CParseValue *data, const char* key)
{
    long index, i, step, hash;

    if (table->size <= table->count)
        cparse_table_rehash(table);


    hash = cparse_table_hash_key(key);
        
    do
    {
        index = hash % table->size;
        step = (hash % (table->size - 2)) + 1;

        for (i = 0; i < table->size; i++)
        {
            if (table->entry[index].flags & ACTIVE)
            {
                if (table->entry[index].hash == hash)
                {
                    table->entry[index].value = cparse_value_copy(data);
                    return;
                }
            }
            else
            {
                table->entry[index].flags |= ACTIVE;
                table->entry[index].value = cparse_value_copy(data);
                table->entry[index].hash = hash;
                table->entry[index].key = strdup(key);
                ++table->count;
                return;
            }

            index = (index + step) % table->size;
        }

        /*
         * it should not be possible that we EVER come this far, but
         * unfortunately not every generated prime number is prime
         * (Carmichael numbers...)
         */
        cparse_table_rehash(table);
    }
    while (1);
}

CParseValue *cparse_table_remove(CParseTable * table, const char* key)
{
    long index, i, step, hash;

    hash = cparse_table_hash_key(key);

    index = hash % table->size;
    step = (hash % (table->size - 2)) + 1;

    for (i = 0; i < table->size; i++)
    {
        if (table->entry[index].value)
        {
            if (table->entry[index].hash == hash)
            {
                if (table->entry[index].flags & ACTIVE)
                {
                    table->entry[index].flags &= ~ACTIVE;
                    --table->count;
                    return table->entry[index].value;
                }
                else	/* in, but not active (i.e. deleted) */
                    return 0;
            }
        }
        else		/* found an empty place (can't be in) */
            return 0;

        index = (index + step) % table->size;
    }
    /*
     * everything searched through, but not in
     */
    return 0;
}

CParseValue *cparse_table_get(CParseTable * table, const char * key)
{
    if (table->count)
    {
        long index, i, step, hash;
        hash = cparse_table_hash_key(key);
        index = hash % table->size;
        step = (hash % (table->size - 2)) + 1;

        for (i = 0; i < table->size; i++)
        {
            if (table->entry[index].hash == hash)
            {
                if (table->entry[index].flags & ACTIVE)
                    return table->entry[index].value;
                break;
            }
            else if (!table->entry[index].value)
                break;

            index = (index + step) % table->size;
        }
    }
    return 0;
}

long cparse_table_count(CParseTable * table)
{
    return table->count;
}

void cparse_table_delete(CParseTable * table)
{
    if (table->count)
    {
        int i;

        for (i = 0; i < table->size; i++)
        {
            if (table->entry[i].value)
                cparse_value_delete(table->entry[i].value);
        }
    }
    free(table->entry);
    free(table);
}

void cparse_table_merge(CParseTable *a, CParseTable *b)
{
    if(b->count)
    {
        int i;

        for(i = 0; i < b->size; i++)
        {
            if(b->entry[i].value)
                cparse_table_insert(a, b->entry[i].value, b->entry[i].key);
        }
    }
}

CParseTable* cparse_table_from_json(json_object *jobj)
{
    CParseTable *table = cparse_table_new(0);

    json_object_object_foreach(jobj, key, val)
    {
        CParseValue *value = cparse_value_from_json(val);

        if(value != NULL) {
            cparse_table_insert(table, value, key);
        }
    }
    return table;
}

static size_t cparse_table_array_to_json(CParseArray *array, char *data, size_t pos)
{
    int i;

    pos += sprintf(&data[pos], "[");

    for(i = 0; i < array->size; i++) {
        pos = cparse_value_to_json(array->value[i], data, pos);
        if(i +1 < array->size) {
            pos += sprintf(&data[pos], ",");
        }
    }

    pos += sprintf(&data[pos], "]");

    return pos;
}

size_t cparse_value_to_json(CParseValue *value, char *data, size_t pos)
{

    switch(cparse_value_type(value))
    {
        case kCPValueNumber:
        pos += sprintf(&data[pos], "%lld", cparse_value_get_number(value));
        break;
        case kCPValueReal:
        pos += sprintf(&data[pos], "%Lf", cparse_value_get_real(value));
        break;
        case kCPValueString:
        pos += sprintf(&data[pos], "\"%s\"", cparse_value_get_string(value));
        break;
        case kCPValueObject:
        pos = cparse_table_to_json(cparse_value_get_object(value), data, pos);
        break;
        case kCPValueArray:
        pos = cparse_table_array_to_json(cparse_value_get_array(value), data, pos);
        break;
    }
    return pos;
}

static size_t cparse_table_entry_to_json(CParseTableEntry *entry, char *data, size_t pos)
{
    pos += sprintf(&data[pos], "\"%s\":", entry->key);

    pos = cparse_value_to_json(entry->value, data, pos);

    return pos;
}

size_t cparse_table_to_json(CParseTable *table, char *data, size_t pos)
{
    assert(table != NULL);

    if(table->count)
    {
        int i, c;

        pos += sprintf(&data[pos], "{");

        for(c = i = 0; i < table->size; i++)
        {
            if(table->entry[i].value)
            {
                pos = cparse_table_entry_to_json(&table->entry[i], data, pos);

                if(++c < table->count) {
                    pos += sprintf(&data[pos], ",");
                }
            }

        }

        pos += sprintf(&data[pos], "}");
    }
    return pos;
}
