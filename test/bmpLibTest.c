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
static const uint32_t HEADER_SIZE = BM_HEADER_SIZE  +
                                    DIB_HEADER_SIZE +
                                    COLOR_TABLE_SIZE;
static const uint32_t PADDING = sizeof(unsigned char) * 3;
static const uint32_t PIXEL_ARRAY_SIZE = TEST_SIZE + PADDING;
static const uint32_t TOTAL_SIZE = HEADER_SIZE + PIXEL_ARRAY_SIZE;


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

#define COLOR_SIZE 4
static const unsigned char WHITE[COLOR_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF};
static const unsigned char BLACK[COLOR_SIZE] = {0x00, 0x00, 0x00, 0x00};
static const uint32_t COLOR_COUNT = 2;

// Custom assert decl

static void assert_color_in_table(
            const unsigned char * colorTable,
            const uint32_t colors,
            const unsigned char color[COLOR_SIZE]);

// Subject under test
static bmp_result * result;

// Setup / teardown
void setup(void) {
    result = create_bw_bmp(&TEST_INPUT, TEST_SIZE, 0);
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

    //Skip reserved bytes
    bmpHeader += DWORD;
    ck_assert_msg(memcmp(&HEADER_SIZE, bmpHeader, DWORD) == 0,
       "Expected offset to bitmap data to be presetn");
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
    ck_assert_msg(memcmp(&PIXEL_ARRAY_SIZE, dibData, DWORD) == 0,
        "Expected Test size to equal size input");
    
    dibData += DWORD;
    const int32_t horzRes = *((int32_t *)dibData);
    ck_assert_msg(horzRes >= 0, 
        "Expected horizontal resolution to be greater than zero");
    
    dibData += DWORD;
    const int32_t vertRes = *((int32_t *)dibData);
    ck_assert_msg(vertRes >= 0, 
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
    assert_color_in_table(colorTable, COLOR_COUNT, BLACK);
    assert_color_in_table(colorTable, COLOR_COUNT, WHITE);
}
END_TEST

START_TEST(test_pixel_array_valid)
{
    const int32_t size = result->dataSize;
    ck_assert_msg(size == TOTAL_SIZE,
        "Size must be equal to expected size");
    
    unsigned char * pixelArray = result->data + HEADER_SIZE;
    ck_assert_msg(memcmp(&TEST_INPUT, pixelArray, TEST_SIZE) == 0,
        "Expected test input to be equal to the pixel array");
 
  
}
END_TEST

#define MAX_WIDTH 8

START_TEST(test_multi_line_output_is_valid) 
{
 
    unsigned char testInput[2] = {0b10101010, 0b01010101};

    bmp_result * result =
       create_bw_bmp(testInput, 2, MAX_WIDTH);

    unsigned char * pixelArray = result->data + HEADER_SIZE;
    
    ck_assert_msg(memcmp(testInput + 1, pixelArray, 1) == 0,
        "Expected second entry of test input to be the first line of output");

    pixelArray += DWORD;
    
    ck_assert_msg(memcmp(testInput, pixelArray, 1) == 0,
        "Expected first entry of test input to be the second line of output");


    if(result != NULL) {
        bmp_free(result);
    }
}
END_TEST

// Custom assert impl
static void assert_color_in_table(
            const unsigned char * colorTable,
            const uint32_t colors,
            const unsigned char color[COLOR_SIZE]) {
    const size_t inc = sizeof(unsigned char) * COLOR_SIZE;
    for(int i = 0; i < colors; i++) {
        if(memcmp(color, colorTable + (i * inc), COLOR_SIZE) == 0) {
            return;
        }
    }
    ck_abort_msg("Unable to find color in color table");
}



// Check boilerplate
static Suite * bmpSuite(void) {
    Suite * s = suite_create("the bmpLib should ");
    TCase * oneLineTest = tcase_create("create output on a single line with max length of zero");
    tcase_add_checked_fixture(oneLineTest, setup, teardown);
    tcase_add_test(oneLineTest, test_created_struct_valid);
    tcase_add_test(oneLineTest, test_bmp_header_valid);
    tcase_add_test(oneLineTest, test_dib_header_valid);
    tcase_add_test(oneLineTest, test_color_table_valid);
    tcase_add_test(oneLineTest, test_pixel_array_valid);
    suite_add_tcase(s, oneLineTest);

    TCase * multiLineTest = tcase_create("create output on multiple lines with a max length greater than zero");
    tcase_add_test(multiLineTest, test_multi_line_output_is_valid);
    suite_add_tcase(s, multiLineTest);

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
