#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include <check.h>

#include "bmpLib.h"

// Constants


static const unsigned char TEST_INPUT = 0b10101010;
static const uint32_t TEST_SIZE = sizeof(unsigned char);
static const uint32_t TEST_BIT_SIZE = TEST_SIZE * CHAR_BIT;

static const size_t WORD = sizeof(uint16_t);
static const size_t DWORD = sizeof(uint32_t);

static const uint32_t BM_HEADER_SIZE = 14;
static const uint32_t DIB_HEADER_SIZE = 40;
static const uint32_t COLOR_TABLE_SIZE = 8;
static const uint32_t TOTAL_SIZE = BM_HEADER_SIZE   +
                                   DIB_HEADER_SIZE  + 
                                   COLOR_TABLE_SIZE +
                                   DWORD; // Padded to 4 bytes

static const unsigned char BM_MAGIC[2] = {0x42, 0x4D}; 
static const uint32_t DIB_HEADER_VAL = DIB_HEADER_SIZE;

static const int32_t EXPECT_WIDTH = TEST_BIT_SIZE;
static const int32_t EXPECT_HEIGHT = 1;

static const uint16_t DIB_COLOR_PLANES = 1;
static const uint16_t COLOR_PLANES = 1;
static const uint16_t COLOR_BITS   = 1; 
static const uint32_t COMPRESSION  = 0; 
static const uint32_t DEFAULT_IMAGE_SIZE = 0;
static const uint32_t DEFAULT_PALETTE_SIZE = 0;
static const uint32_t DEFAULT_VIP_COLORS   = 0;
static const unsigned char WHITE[4]    = {0xFF, 0xFF, 0xFF, 0xFF};
static const unsigned char BLACK[4]    = {0x00, 0x00, 0x00, 0x00};


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


//Tests go here
START_TEST(test_created_struct_valid) 
{
    ck_assert_msg(result != NULL, "Result cannot be NULL");
    ck_assert_msg(result->data != NULL, "Result data cannot be NULL");
    ck_assert_msg(result->dataSize >= 0, "Result size must be ge 0");
}

END_TEST

START_TEST(test_bmp_header_valid) 
{
    const int32_t size = result->dataSize;
    const unsigned char * bmpHeader = result->data;

    ck_assert_msg(size >= BM_HEADER_SIZE, 
        "Size must be greater than BM_HEADER_SIZE");
    ck_assert_msg(memcmp(BM_MAGIC, bmpHeader, sizeof(BM_MAGIC)) ==  0,
        "BMP magic number not found");
    
    bmpHeader += sizeof(BM_MAGIC);
    ck_assert_msg(memcmp(&TOTAL_SIZE, bmpHeader, DWORD) == 0,
       "Exepcted size does not match expectation");

    bmpHeader += DWORD;

    //Skip resevered bytes
    bmpHeader += DWORD;
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
    ck_assert_msg(memcmp(&COMPRESSION, dibData, DWORD) == 0,
        "Expected compression to be disabled");
 
    dibData += DWORD;
    ck_assert_msg(memcmp(&DEFAULT_IMAGE_SIZE, dibData, DWORD) == 0,
        "Expected Test size to equal size input");
    
    dibData += DWORD;
    const int32_t horzRes = *((int32_t *)dibData);
    ck_assert_msg(horzRes > 0, 
        "Expected horizontal resolution to be greater than zero");
    
    dibData += DWORD;
    const int32_t vertRes = *((int32_t *)dibData);
    ck_assert_msg(vertRes > 0, 
        "Expected vertial resolution to be greater than zero");

    dibData += DWORD;
    ck_assert_msg(memcmp(&DEFAULT_PALETTE_SIZE, dibData, DWORD) == 0,
        "Expected default color palette to be used");

    dibData += DWORD;
    ck_assert_msg(memcmp(&DEFAULT_VIP_COLORS, dibData, DWORD) == 0,
        "Expected no important colors to be defined");
}
END_TEST

START_TEST(test_color_table_valid)
{
    const int32_t size = result->dataSize;
    ck_assert_msg(size >= BM_HEADER_SIZE + DIB_HEADER_SIZE + COLOR_TABLE_SIZE, 
        "Size must be greater than BM_HEADER_SIZE + DIB_HEADER_SIZE + COLOR_TABLE_SIZE");

    const unsigned char * colorTable = result->data   + 
                                       BM_HEADER_SIZE +
                                       DIB_HEADER_SIZE;
    ck_assert_msg(memcmp(WHITE, colorTable, sizeof(WHITE)) == 0,
         "Expected white to be first entry in color table");

    colorTable += sizeof(WHITE);
    ck_assert_msg(memcmp(BLACK, colorTable, sizeof(BLACK)) == 0,
         "Expected black to be first entry in color table");
}
END_TEST

// Check boilerplate
static Suite * bmpSuite(void) {
    Suite * s = suite_create("bmpLib image tests");
    TCase * tc_simple = tcase_create("lib creates valid images");
    tcase_add_checked_fixture(tc_simple, setup, teardown);
    tcase_add_test(tc_simple, test_created_struct_valid);
    tcase_add_test(tc_simple, test_bmp_header_valid);
    tcase_add_test(tc_simple, test_dib_header_valid);
    tcase_add_test(tc_simple, test_color_table_valid);
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
