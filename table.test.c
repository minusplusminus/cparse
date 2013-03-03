#include <check.h>
#include <stdio.h>
#include <cparse/table.h>

CParseTable *cp_table;

static void cparse_test_setup()
{
	cp_table = cparse_table_new(0);
}

static void cparse_test_teardown()
{
	cparse_table_delete(cp_table);
}

START_TEST(test_cparse_table_insert)
{
	CParseValue *value = cparse_value_new();

	cparse_value_set_number(value, 1234);

	cparse_table_insert(cp_table, value, "id");

	fail_unless(cparse_table_count(cp_table) == 1);

	fail_unless(cparse_value_get_number(cparse_table_get(cp_table, "id")) == 1234);
}
END_TEST

START_TEST(test_cparse_table_count)
{
	CParseValue *value = cparse_value_new();

	cparse_value_set_string(value, "1234");

	cparse_table_insert(cp_table, value, "teststr");

	value = cparse_value_new();

	cparse_value_set_real(value, 1234.5678);

	cparse_table_insert(cp_table, value, "testreal");

	fail_unless(cparse_table_count(cp_table) == 2);
}
END_TEST

START_TEST(test_cparse_table_remove)
{
	CParseValue *value = cparse_value_new();

	cparse_value_set_string(value, "hello, world");

	cparse_table_insert(cp_table, value, "main");

	fail_unless(cparse_table_count(cp_table) == 1);

	CParseValue *removed = cparse_table_remove(cp_table, "main");

	fail_unless(cparse_value_equals(removed, value));

	fail_unless(cparse_table_count(cp_table) == 0);

}
END_TEST

START_TEST(test_cparse_table_to_json)
{
	char buf[BUFSIZ];

	CParseValue value;

	cparse_value_set_string(&value, "Hello,World");

	cparse_table_insert(cp_table, &value, "main");

	cparse_table_to_json(cp_table, buf, 0);

	fail_unless(!strcmp(buf, "{\"main\":\"Hello,World\"}"));

	cparse_value_set_number(&value, 1234);

	cparse_table_insert(cp_table, &value, "main");

	cparse_table_to_json(cp_table, buf, 0);

	fail_unless(!strcmp(buf, "{\"main\":1234}"));
}
END_TEST

Suite *cparse_table_suite (void)
{
    Suite *s = suite_create ("Table");

    /* Core test case */
    TCase *tc = tcase_create ("Table");
    tcase_add_checked_fixture(tc, cparse_test_setup, cparse_test_teardown);
    tcase_add_test(tc, test_cparse_table_insert);
    tcase_add_test(tc, test_cparse_table_count);
    tcase_add_test(tc, test_cparse_table_remove);
    tcase_add_test(tc, test_cparse_table_to_json);
    suite_add_tcase(s, tc);

    return s;
}