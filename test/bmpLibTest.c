#include <stdlib.h>
#include <stdio.h>

#include <check.h>

#include "bmpLib.h"

static void assert_struct_nerr(const bmp_result * const bmp) {
    ck_assert_msg(bmp != NULL, "Result cannot be NULL");
    ck_assert_msg(bmp->resultSize >= 0, "Result size must be ge 0");
    ck_assert_msg(bmp->result != NULL, "Result data cannot be NULL");
}

START_TEST(test_generates_valid_result) 
{
    const unsigned char input = 0xAA; // 10101010
    bmp_result * bmp = create_bw_bmp(&input, 1);
    assert_struct_nerr(bmp);
    bmp_free(bmp);
}
END_TEST

static Suite * bmpSuite(void) {
    Suite * s = suite_create("bmpLib");
    TCase * tc_simple = tcase_create("Simple tests");
    tcase_add_test(tc_simple, test_generates_valid_result);
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
