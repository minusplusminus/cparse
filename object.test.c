#include <check.h>
#include <stdio.h>
#include <cparse/object.h>
#include <cparse/parse.h>
#include <cparse/value.h>

CParseObject *cp_obj = NULL;

static void cparse_test_setup()
{
    cp_obj = cparse_object_with_class_name("TestCase");
}

static void cparse_test_teardown()
{
    cparse_object_free(cp_obj);
}

START_TEST(test_cparse_object_save)
{
	cparse_set_application_id("CODG0SmrNhqoZOheWT0Q4sATFvQTdZhzOjGA5OGb");

	cparse_set_api_key("wXu2P8buzDdTq02mQgJkxrJz1TEHNgOvKtDTUuZ7");

	cparse_object_set_number(cp_obj, "score", 1234);

	cparse_object_set_string(cp_obj, "name", "spok");

    CParseError *error = NULL;

	cparse_object_save(cp_obj, &error);

    fail_unless(error == NULL);

    fail_unless(cp_obj->objectId != NULL);
}
END_TEST


START_TEST(test_cparse_object_set_value)
{
    cparse_object_set_number(cp_obj, "id", 1234);

    fail_unless(cparse_object_attribute_count(cp_obj) == 1);

    fail_unless(cparse_object_get_number(cp_obj, "id", 0) == 1234);
}
END_TEST

START_TEST(test_cparse_object_count_attributes)
{
    cparse_object_set_string(cp_obj, "teststr", "1234");

    cparse_object_set_real(cp_obj, "testreal", 1234.5678);

    fail_unless(cparse_object_attribute_count(cp_obj) == 2);
}
END_TEST

START_TEST(test_cparse_object_remove_attribute)
{
    CParseValue value;

    cparse_value_set_string(&value, "1234");

    cparse_object_set_value(cp_obj, "main", &value);

    fail_unless(cparse_object_attribute_count(cp_obj) == 1);

    CParseValue *removed = cparse_object_remove_attribute(cp_obj, "main");

    fail_unless(cparse_value_equals(removed, &value));

    fail_unless(cparse_object_attribute_count(cp_obj) == 0);

}
END_TEST

START_TEST(test_cparse_object_to_json)
{
    char buf[BUFSIZ];

    cparse_object_set_string(cp_obj, "main", "Hello,World");

    cparse_object_to_json(cp_obj, buf, 0);

    fail_unless(!strcmp(buf, "{\"main\":\"Hello,World\"}"));

    cparse_object_set_number(cp_obj, "main", 1234);

    cparse_object_to_json(cp_obj, buf, 0);

    fail_unless(!strcmp(buf, "{\"main\":1234}"));
}
END_TEST

Suite *cparse_object_suite (void)
{
    Suite *s = suite_create ("Object");

    /* Core test case */
    TCase *tc = tcase_create ("Object");
    tcase_add_checked_fixture(tc, cparse_test_setup, cparse_test_teardown);
    tcase_add_test(tc, test_cparse_object_save);
    tcase_add_test(tc, test_cparse_object_set_value);
    tcase_add_test(tc, test_cparse_object_count_attributes);
    tcase_add_test(tc, test_cparse_object_remove_attribute);
    tcase_add_test(tc, test_cparse_object_to_json);
    suite_add_tcase(s, tc);

    return s;
}