#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#include <check.h>

#include "bmpLib.h"

// Constants

#define BM_HEADER_SIZE 14
#define DIB_HEADER_SIZE 40

static const unsigned char BM_MAGIC[2] = {0x42, 0x4D}; 

// Binary for 10101010
// so black, white, black white etc.
static const unsigned char TEST_INPUT = 0xAA;
static bmp_result * result;

// Setup / teardown
void setup(void) {
    result = create_bw_bmp(&TEST_INPUT, 1);
}

void teardown(void) {
    if(result != NULL) {
        bmp_free(result);
    }
}

// Custom assert decls
static void assert_struct_nerr(const bmp_result * const bmp);

//Tests go here
START_TEST(test_created_struct_valid) 
{
    assert_struct_nerr(result);
}
END_TEST

START_TEST(test_bmp_header_valid) 
{
    const int32_t size = result->dataSize;
    const unsigned char * data = result->data;
    ck_assert_msg(size >= BM_HEADER_SIZE, "Size must be greater than BM_HEADER_SIZE");
    ck_assert_msg(memcmp(BM_MAGIC, data, sizeof(BM_MAGIC)) ==  0, "BMP magic number not found");
}
END_TEST

START_TEST(test_dib_header_valid) 
{
    const int32_t size = result->dataSize;
    ck_assert_msg(size >= BM_HEADER_SIZE + DIB_HEADER_SIZE, "Size must be greater than BM_HEADER_SIZE + DIB_HEADER_SIZE (BMP header + DIB Header)");
    const unsigned char * dibData = result->data + BM_HEADER_SIZE;
    const uint32_t headerSize = DIB_HEADER_SIZE;
    ck_assert_msg(memcmp(&headerSize, dibData, sizeof(uint32_t)) ==  0, "Expecting header size to be present as first entry in dib header");
}
END_TEST



// Custom asserts impls
static void assert_struct_nerr(const bmp_result * const bmp) {
    ck_assert_msg(bmp != NULL, "Result cannot be NULL");
    ck_assert_msg(bmp->data != NULL, "Result data cannot be NULL");
    ck_assert_msg(bmp->dataSize >= 0, "Result size must be ge 0");
}


// Check boilerplate
static Suite * bmpSuite(void) {
    Suite * s = suite_create("bmpLib image tests");
    TCase * tc_simple = tcase_create("lib creates valid images");
    tcase_add_checked_fixture(tc_simple, setup, teardown);
    tcase_add_test(tc_simple, test_created_struct_valid);
    tcase_add_test(tc_simple, test_bmp_header_valid);
    tcase_add_test(tc_simple, test_dib_header_valid);
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
