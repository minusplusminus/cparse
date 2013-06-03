#ifndef CPARSE_ACL_H_
#define CPARSE_ACL_H_

#include <cparse/defines.h>

#ifdef __cplusplus
extern "C" {
#endif

    struct cparse_acl
    {
        CParseACL *next;
        char *name;
        bool read;
        bool write;
    };

    CParseACL *cparse_acl_new();

    CParseACL *cparse_acl_new_with_user(CParseUser *user);

    CParseACL *cparse_acl_copy(CParseACL *other);

    void cparse_set_default_acl(CParseACL *acl, bool currentUserAccess);

    void cparse_acl_free(CParseACL *acl);


#ifdef __cplusplus
}
#endif

#endif
