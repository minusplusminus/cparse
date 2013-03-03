#include <cparse/value.h>
#include <cparse/table.h>
#include <check.h>

static CParseValue *cpv_test;

static void cparse_test_setup()
{
	cpv_test = cparse_value_new();
}

static void cparse_test_teardown()
{
	cparse_value_delete(cpv_test);
}

START_TEST(test_cparse_value_set_number)
{
	cparse_value_set_number(cpv_test, 1234);

	fail_unless(cpv_test->number_value == 1234);
}
END_TEST

START_TEST(test_cparse_value_set_real)
{
	cparse_value_set_real(cpv_test, 1234.4321);

	fail_unless(cpv_test->real_value == 1234.4321);
}
END_TEST

START_TEST(test_cparse_value_set_string)
{
	cparse_value_set_string(cpv_test, "1234");

	fail_unless(!strcmp(cparse_value_get_string(cpv_test), "1234"));
}
END_TEST

START_TEST(test_cparse_value_set_object)
{
	CParseTable *obj = cparse_table_new(0);

	cparse_value_set_object(cpv_test, obj);

	fail_unless(cpv_test->object_value == obj);
}
END_TEST

START_TEST(test_cparse_value_set_array)
{
	CParseArray *array = cparse_array_new();

	CParseValue inner;

	cparse_value_set_number(&inner, 1234);

	cparse_array_add(array, &inner);

	cparse_value_set_array(cpv_test, array);

	fail_unless(cparse_value_get_array(cpv_test) == array);
}
END_TEST

START_TEST(test_cparse_array_add)
{
	CParseArray *array = cparse_array_new();

	cparse_value_set_number(cpv_test, 1234);

	cparse_array_add(array, cpv_test);

	fail_unless(cparse_array_size(array) == 1);

}
END_TEST

Suite *cparse_value_suite (void)
{
    Suite *s = suite_create ("Value");

    /* Core test case */
    TCase *tc = tcase_create ("Value");
    tcase_add_checked_fixture(tc, cparse_test_setup, cparse_test_teardown);
    tcase_add_test(tc, test_cparse_value_set_number);
    tcase_add_test(tc, test_cparse_value_set_real);
    tcase_add_test(tc, test_cparse_value_set_string);
    tcase_add_test(tc, test_cparse_value_set_object);
    tcase_add_test(tc, test_cparse_value_set_array);
    suite_add_tcase(s, tc);

    tc = tcase_create("Array");
    tcase_add_checked_fixture(tc, cparse_test_setup, cparse_test_teardown);
    tcase_add_test(tc, test_cparse_array_add);
    suite_add_tcase(s, tc);

    return s;
}