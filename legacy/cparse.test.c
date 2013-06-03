
#include <check.h>
#include <stdlib.h>
#include <cparse/parse.h>

Suite *cparse_parse_suite();
Suite *cparse_json_suite();
Suite *cparse_object_suite();

int main()
{
    int number_failed;
    SRunner *sr = srunner_create(cparse_parse_suite());
    srunner_add_suite(sr, cparse_json_suite());
    srunner_add_suite(sr, cparse_object_suite());
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
