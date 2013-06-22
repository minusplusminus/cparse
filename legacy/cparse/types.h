#ifndef CPARSE_TYPES_H_
#define CPARSE_TYPES_H_

#include <cparse/defines.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct cparse_type_bytes
    {
        void *data;
        size_t size;
    };

    struct cparse_type_date
    {
        time_t value;
    };

    struct cparse_type_file
    {
        char *localFileName;
        char *parseFileName;
        char *contentType;
        char *body;
        char *url;
    };

    struct cparse_type_geopoint
    {
        double latitude;
        double longitude;
    };

    struct cparse_type_pointer
    {
        char *className;
        char *objectId;
    };

    CParsePointer *cparse_pointer_from_json(CParseJSON *data);

    CParseJSON *cparse_pointer_to_json(CParsePointer *p);

    void cparse_pointer_free(CParsePointer *pointer);

    CParseBytes *cparse_bytes_from_json(CParseJSON *data);

    void cparse_bytes_free(CParseBytes *bytes);

    CParseFile *cparse_file_from_json(CParseJSON *data);

    void cparse_file_free(CParseFile *file);


#ifdef __cplusplus
}
#endif

#endif
