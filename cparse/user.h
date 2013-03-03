#ifndef CPARSE_USER_H_
#define CPARSE_USER_H_

#include <stdbool.h>
#include <cparse/defines.h>
#include <cparse/query.h>

struct cparse_user
{
	char *username;
	char *email;
	char *password;
	char *sessionToken;
	bool isNew;
};

CParseUser *cparse_current_user();

void cparse_user_enable_automatic_user();

CParseUser *cparse_user_login(const char *username, const char *password, CParseError **error);

void cparse_user_login_in_background(const char *username, const char *password, CParseUserCallback callback);

void cparse_user_logout();

CParseQuery *cparse_user_query();

#endif
