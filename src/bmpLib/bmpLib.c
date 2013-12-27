#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "bmpLib.h"

static const uint32_t BM_HEADER_SIZE = 14;
static const uint32_t DIB_HEADER_SIZE = 40;
static const unsigned char BM_MAGIC[2] = {0x42, 0x4D};
static const uint32_t DIB_HEADER_VAL = DIB_HEADER_SIZE;
static const uint32_t HEADERS_SIZE = BM_HEADER_SIZE + DIB_HEADER_SIZE;

bmp_result * create_bw_bmp(const unsigned char * data, const uint32_t dataLen) {
    bmp_result * const result = (bmp_result *)calloc(1, sizeof(bmp_result));
    result->dataSize = HEADERS_SIZE;
    result->data = (unsigned char *)calloc(HEADERS_SIZE, sizeof(unsigned char));

    memcpy(result->data, BM_MAGIC, sizeof(BM_MAGIC));
    memcpy(result->data + BM_HEADER_SIZE, &DIB_HEADER_VAL, sizeof(uint32_t));
    return result;
}


void bmp_free(bmp_result * result) {
    result->dataSize = 0;
    free(result->data);
    free(result); 
}
