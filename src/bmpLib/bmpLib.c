#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "bmpLib.h"

// BMP / DIB related constants
#define COLOR_PLANES 1
#define COLOR_BITS 1
#define COMPRESSION 0 
#define DEFAULT_IMAGE_SIZE 0 
#define RESOLUTION 0
#define PALETTE_SIZE 0
#define VIP_COLORS 0
#define PIXEL_PADDING 4

static const uint32_t BM_HEADER_SIZE = 14;
static const uint32_t DIB_HEADER_SIZE = 40;
static const uint32_t DIB_HEADER_VAL = DIB_HEADER_SIZE;
static const unsigned char BM_MAGIC[2] = {0x42, 0x4D}; // 'BM'
static const unsigned char WHITE[4]    = {0xFF, 0xFF, 0xFF, 0xFF};
static const unsigned char BLACK[4]    = {0x00, 0x00, 0x00, 0x00};
static const uint32_t COLOR_TABLE_SIZE = sizeof(WHITE) +
                                         sizeof(BLACK);
static const uint32_t HEADERS_SIZE = BM_HEADER_SIZE + 
                                     DIB_HEADER_SIZE +  
                                     COLOR_TABLE_SIZE;

// Convience constants
static const size_t DWORD = sizeof(uint32_t);
static const size_t WORD  = sizeof(uint16_t);

// Function decls 

static uint32_t render_bw_pixels(const unsigned char * data,
                                 const uint32_t dataLen, 
                                 const int32_t pixelWidth, 
                                 const int32_t pixelHeight, 
                                 unsigned char ** pixelArray);

static int32_t bw_pixel_width(uint32_t dataLen, uint32_t maxWidth);
static int32_t bw_pixel_height(uint32_t dataLen, int32_t pixelWidth);

static void memcpy_uint(unsigned char * loc, uint32_t value);
static void memcpy_suint(unsigned char * loc, uint16_t value);
static void memcpy_int(unsigned char * loc, int32_t value);

bmp_result * create_bw_bmp(const unsigned char * data, 
                           const uint32_t dataLen,
                           const uint32_t maxWidth) {
    unsigned char * pixelArray = NULL;
    const uint32_t pixelWidth = bw_pixel_width(dataLen, maxWidth);
    const uint32_t pixelHeight = bw_pixel_height(dataLen, pixelWidth);
    const uint32_t pixelArraySize = 
        render_bw_pixels(data, dataLen, pixelWidth, pixelHeight, &pixelArray);

    const uint32_t totalSize = HEADERS_SIZE + pixelArraySize;

    unsigned char * resultData = 
        (unsigned char *)malloc(totalSize * sizeof(unsigned char));
    unsigned char * bmpHeader = resultData;    

    memcpy(bmpHeader, BM_MAGIC, sizeof(BM_MAGIC));

    bmpHeader += sizeof(BM_MAGIC);
    memcpy_uint(bmpHeader, totalSize);

    bmpHeader += DWORD;
    //Zero out reserved fields
    memset(bmpHeader, 0, DWORD);
    bmpHeader += DWORD;

    memcpy_uint(bmpHeader, HEADERS_SIZE);

    unsigned char * dibHeader = resultData + BM_HEADER_SIZE;
  
    memcpy_uint(dibHeader, DIB_HEADER_SIZE);
    dibHeader += DWORD;
    memcpy_int(dibHeader, pixelWidth);
    dibHeader += DWORD;
    memcpy_int(dibHeader, pixelHeight);
    dibHeader += DWORD;
    memcpy_suint(dibHeader, COLOR_PLANES);
    dibHeader += WORD;
    memcpy_suint(dibHeader, COLOR_BITS);
    dibHeader += WORD;
    memcpy_uint(dibHeader, COMPRESSION);
    dibHeader += DWORD;
    memcpy_uint(dibHeader, pixelArraySize);
    // Horizontal
    dibHeader += DWORD;
    memcpy_int(dibHeader, RESOLUTION);
    // Vertical
    dibHeader += DWORD;
    memcpy_int(dibHeader, RESOLUTION);
    dibHeader += DWORD;
    memcpy_uint(dibHeader, PALETTE_SIZE);
    dibHeader += DWORD;
    memcpy_uint(dibHeader, VIP_COLORS);

    unsigned char * colorTable = dibHeader + DWORD;
    memcpy(colorTable, BLACK, sizeof(BLACK));

    colorTable += sizeof(BLACK);
    memcpy(colorTable, WHITE, sizeof(WHITE));

    unsigned char * pixelArrayPtr = colorTable + sizeof(WHITE);
    memcpy(pixelArrayPtr, pixelArray, pixelArraySize); 
    free(pixelArray);

    bmp_result * const result = (bmp_result *)malloc(1 * sizeof(bmp_result));
    result->data = resultData;
    result->dataSize = totalSize;
    return result;
}

void bmp_free(bmp_result * result) {
    result->dataSize = 0;
    free(result->data);
    free(result); 
}

static uint32_t render_bw_pixels(const unsigned char * data, 
                                 const uint32_t dataLen, 
                                 const int32_t pixelWidth,
                                 const int32_t pixelHeight,
                                 unsigned char ** pixelArray) {
    const uint32_t bytePerRow = pixelWidth / CHAR_BIT;
    const uint32_t rowSize = bytePerRow % PIXEL_PADDING == 0 ? 
                             bytePerRow :
                             bytePerRow + (PIXEL_PADDING - (bytePerRow % PIXEL_PADDING));
    const uint32_t arraySize = rowSize * pixelHeight;
    unsigned char * result = (unsigned char *)calloc(arraySize, sizeof(unsigned char));
    for(int i = 0; i < pixelHeight; i++) {
        memcpy(result + (rowSize * i), 
               (data + dataLen) - (bytePerRow * (i + 1)), 
               bytePerRow);
    }
    (*pixelArray) = result;
    return arraySize;
}

static int32_t bw_pixel_width(uint32_t dataLen, uint32_t maxWidth) {
    const uint32_t dataLenPixels = dataLen * CHAR_BIT;
    if(maxWidth == 0 || dataLenPixels <= maxWidth) {
        return dataLenPixels;
    }
    else {
        return maxWidth;
    }
}

static int32_t bw_pixel_height(uint32_t dataLen, int pixelWidth) {
    return (dataLen * CHAR_BIT) / pixelWidth;
}

static void memcpy_uint(unsigned char * loc, uint32_t value) {
    *((uint32_t *)loc) = value;
}

static void memcpy_suint(unsigned char * loc, uint16_t value) {
    *((uint16_t *)loc) = value;
}

static void memcpy_int(unsigned char * loc, int32_t value) {
    *((int32_t *)loc) = value;
}

