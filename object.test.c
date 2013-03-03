#include <check.h>
#include <cparse/object.h>
#include <cparse/parse.h>

CParseObject *cp_obj = NULL;

static void cparse_test_setup()
{
}

static void cparse_test_teardown()
{
}

START_TEST(test_cparse_object_save)
{
	cparse_set_application_id("CODG0SmrNhqoZOheWT0Q4sATFvQTdZhzOjGA5OGb");

	cparse_set_api_key("wXu2P8buzDdTq02mQgJkxrJz1TEHNgOvKtDTUuZ7");

	CParseObject *obj = cparse_object_with_class_name("SpaceCadets");

	cparse_object_set_number(obj, "score", 1234);

	cparse_object_set_string(obj, "name", "spok");

	cparse_object_save(obj, NULL);
}
END_TEST

Suite *cparse_object_suite (void)
{
    Suite *s = suite_create ("Object");

    /* Core test case */
    TCase *tc = tcase_create ("Object");
    tcase_add_checked_fixture(tc, cparse_test_setup, cparse_test_teardown);
    tcase_add_test(tc, test_cparse_object_save);
    suite_add_tcase(s, tc);

    return s;
}