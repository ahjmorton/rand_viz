#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include <check.h>

#include "bmpLib.h"

// Constants

static const size_t WORD = sizeof(uint16_t);
static const size_t DWORD = sizeof(uint32_t);
static const uint32_t BM_HEADER_SIZE = 14;
static const uint32_t DIB_HEADER_SIZE = 40;
static const uint32_t DIB_HEADER_VAL = DIB_HEADER_SIZE;
static const uint16_t DIB_COLOR_PLANES = 1;
static const unsigned char BM_MAGIC[2] = {0x42, 0x4D}; 
// Binary for 10101010
// so black, white, black white etc.
static const unsigned char TEST_INPUT = 0xAA;
static const uint32_t TEST_SIZE = sizeof(unsigned char);
static const uint32_t TEST_BIT_SIZE = TEST_SIZE * CHAR_BIT;
static const int32_t EXPECT_WIDTH = TEST_BIT_SIZE;
static const int32_t EXPECT_HEIGHT = 1;
static const uint16_t COLOR_PLANES = 1;
static const uint16_t COLOR_BITS   = 1; 

static bmp_result * result;

// Setup / teardown
void setup(void) {
    result = create_bw_bmp(&TEST_INPUT, TEST_SIZE);
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

    ck_assert_msg(size >= BM_HEADER_SIZE, 
        "Size must be greater than BM_HEADER_SIZE");
    ck_assert_msg(memcmp(BM_MAGIC, data, sizeof(BM_MAGIC)) ==  0,
        "BMP magic number not found");
}
END_TEST

START_TEST(test_dib_header_valid) 
{
    const int32_t size = result->dataSize;
    ck_assert_msg(size >= BM_HEADER_SIZE + DIB_HEADER_SIZE, 
        "Size must be greater than BM_HEADER_SIZE + DIB_HEADER_SIZE (BMP header + DIB Header)");

    const unsigned char * dibData = result->data + BM_HEADER_SIZE;
    ck_assert_msg(memcmp(&DIB_HEADER_VAL, dibData, DWORD) == 0, 
        "Expecting header size to be present as first entry in dib header");

    dibData += DWORD;

    ck_assert_msg(memcmp(&EXPECT_WIDTH, dibData, DWORD) == 0, 
        "Expected width to equal ");

    dibData += DWORD;

    ck_assert_msg(memcmp(&EXPECT_HEIGHT, dibData, DWORD) == 0, 
        "Expected height to equal ");

    dibData += DWORD;

    ck_assert_msg(memcmp(&COLOR_PLANES, dibData, WORD) == 0,
        "Only expected 1 color plane");

    dibData += WORD;

    ck_assert_msg(memcmp(&COLOR_BITS, dibData, WORD) == 0,
        "Only expected 1 color bit to be specified");

    dibData += WORD;

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
