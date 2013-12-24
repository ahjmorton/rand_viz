#include <stdlib.h>

#include <check.h>

#include "bmpLib.h"

START_TEST(test_simple_image) 
{
    
}
END_TEST


static Suite * bmpSuite(void) {
    Suite * s = suite_create("bmpLib");
    TCase * tc_simple = tcase_create("Simple tests");
    tcase_add_test(tc_simple, test_simple_image);
    suite_add_tcase(s, tc_simple);
    return s;
}

int main (void) {
    int number_failed;
    Suite * s = bmpSuite();
    SRunner * sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
