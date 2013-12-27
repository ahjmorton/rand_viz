#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "bmpLib.h"

static const uint32_t BM_HEADER_SIZE = 14;
static const uint32_t DIB_HEADER_SIZE = 40;
static const unsigned char BM_MAGIC[2] = {0x42, 0x4D};
static const uint32_t DIB_HEADER_VAL = DIB_HEADER_SIZE;
static const uint32_t HEADERS_SIZE = BM_HEADER_SIZE + DIB_HEADER_SIZE;
static const size_t DWORD = sizeof(uint32_t);
static const size_t WORD  = sizeof(uint16_t);

static int32_t bw_pixel_width(uint32_t dataLen);
static int32_t bw_pixel_height(uint32_t dataLen);

static void memcpy_uint(unsigned char * loc, uint32_t value);
static void memcpy_int(unsigned char * loc, int32_t value);

bmp_result * create_bw_bmp(const unsigned char * data, const uint32_t dataLen) {
    bmp_result * const result = (bmp_result *)calloc(1, sizeof(bmp_result));

    unsigned char * bmpHeader = 
        (unsigned char *)calloc(HEADERS_SIZE, sizeof(unsigned char));
    result->data = bmpHeader;
    result->dataSize = HEADERS_SIZE;
    
    memcpy(bmpHeader, BM_MAGIC, sizeof(BM_MAGIC));

    unsigned char * dibHeader = bmpHeader + BM_HEADER_SIZE;
  
    memcpy_uint(dibHeader, DIB_HEADER_SIZE);
    dibHeader += DWORD;
    memcpy_int(dibHeader, bw_pixel_width(dataLen));
    dibHeader += DWORD;
    memcpy_int(dibHeader, bw_pixel_height(dataLen));
  
    return result;
}

void bmp_free(bmp_result * result) {
    result->dataSize = 0;
    free(result->data);
    free(result); 
}

static int32_t bw_pixel_width(uint32_t dataLen) {
    return dataLen * CHAR_BIT;
}

static int32_t bw_pixel_height(uint32_t dataLen) {
    return 1;
}

static void memcpy_uint(unsigned char * loc, uint32_t value) {
    *((uint32_t *)loc) = value;
}

static void memcpy_int(unsigned char * loc, int32_t value) {
    *((int32_t *)loc) = value;
}

