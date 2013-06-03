#include <stdlib.h>
#include <string.h>
#include <cparse/user.h>


CParseUser *current_user;

CParseUser *automatic_user;

extern CParseQuery *cparse_query_new();

CParseUser *cparse_current_user()
{
    if(current_user == NULL)
    {
        return automatic_user;
    }
    return current_user;
}

static CParseUser *cparse_user_new()
{
    CParseUser *user = malloc(sizeof(CParseUser));

    user->username = NULL;
    user->email = NULL;
    user->password = NULL;
    user->sessionToken = NULL;
    user->isNew = true;
    return user;
}

void cparse_user_delete(CParseUser *user)
{
    if(user->username)
        free(user->username);
    if(user->email)
        free(user->email);
    if(user->password)
        free(user->password);
    if(user->sessionToken)
        free(user->sessionToken);

    free(user);
}

void cparse_user_enable_automatic_user()
{
    if(automatic_user == NULL)
    {
        automatic_user = cparse_user_new();
    }
}

CParseUser *cparse_user_login(const char *username, const char *password, CParseError **error)
{
    CParseUser *user = cparse_user_new();

    user->username = strdup(username);
    user->password = strdup(password);

    /* TODO: do network logic and set error */


    return user;
}

void cparse_user_login_in_background(const char *username, const char *password, CParseUserCallback callback)
{

}

void cparse_user_logout()
{

}

CParseQuery *cparse_user_query()
{
    CParseQuery *query = cparse_query_new();

    return query;
}
