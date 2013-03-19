#include <stdlib.h>
#include <string.h>
#include <cparse/acl.h>
#include <cparse/user.h>

CParseACL *first_acl;

CParseACL *default_acl = 0;


CParseACL *cparse_acl_new() {
	CParseACL *acl = malloc(sizeof(CParseACL));
	acl->next = NULL;
	acl->name = NULL;
	acl->read = false;
	acl->write = false;
	return acl;
}

CParseACL *cparse_acl_new_with_user(CParseUser *user) {
	CParseACL *acl = cparse_acl_new();

	acl->name = strdup(user->username);

	return acl;
}

CParseACL *cparse_acl_copy(CParseACL *other) {
	CParseACL *acl = cparse_acl_new();

	acl->name = strdup(other->name);
	acl->read = other->read;
	acl->write = other->write;

	return acl;
}

void cparse_set_default_acl(CParseACL *acl, bool currentUserAccess) {
	default_acl = acl;

	if(currentUserAccess) {

	}
}

void cparse_acl_free(CParseACL *acl) {
	if(acl->name) {
		free(acl->name);
	}
	free(acl);
}