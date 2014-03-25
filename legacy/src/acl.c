#include <stdlib.h>
#include <string.h>
#include <cparse/acl.h>
#include <cparse/user.h>

CPARSE_ACL *first_acl;

CPARSE_ACL *default_acl = 0;


CPARSE_ACL *cparse_acl_new()
{
    CPARSE_ACL *acl = malloc(sizeof(CPARSE_ACL));
    acl->next = NULL;
    acl->name = NULL;
    acl->read = false;
    acl->write = false;
    return acl;
}

CPARSE_ACL *cparse_acl_new_with_user(CPARSE_USER *user)
{
    CPARSE_ACL *acl = cparse_acl_new();

    acl->name = strdup(user->username);

    return acl;
}

CPARSE_ACL *cparse_acl_copy(CPARSE_ACL *other)
{
    CPARSE_ACL *acl = cparse_acl_new();

    acl->name = strdup(other->name);
    acl->read = other->read;
    acl->write = other->write;

    return acl;
}

void cparse_set_default_acl(CPARSE_ACL *acl, bool currentUserAccess)
{
    default_acl = acl;

    if (currentUserAccess)
    {

    }
}

void cparse_acl_free(CPARSE_ACL *acl)
{
    if (acl->name)
    {
        free(acl->name);
    }
    free(acl);
}