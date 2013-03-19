#include <stdlib.h>
#include <cparse/parse.h>

const char *cparse_app_id = NULL;

const char *cparse_api_key = NULL;

const char *cparse_facebook_app_id = NULL;

bool cparse_offline_messages = false;

bool cparse_error_messages = false;

void cparse_set_application_id(const char *appId) {
	cparse_app_id = appId;
}

void cparse_set_api_key(const char *apiKey) {
	cparse_api_key = apiKey;
}

void cparse_set_facebook_application_id(const char *appId) {
	cparse_facebook_app_id = appId;
}

bool cparse_has_facebook_application_id() {
	return cparse_facebook_app_id != 0;
}

void cparse_offline_messages_enabled(bool value) {
	cparse_offline_messages = value;
}

void cparse_error_messages_enabled(bool value) {
	cparse_error_messages = value;
}
