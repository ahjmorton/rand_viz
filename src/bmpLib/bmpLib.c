#include <stdlib.h>
#include <string.h>

#include "bmpLib.h"

#define BM_HEADER_SIZE 14

static const unsigned char BM_MAGIC[2] = {0x42, 0x4D}; 

bmp_result * create_bw_bmp(const unsigned char * data, const unsigned int dataLen) {
    bmp_result * result = (bmp_result *)calloc(1, sizeof(bmp_result));
    result->dataSize = BM_HEADER_SIZE;
    result->data = (unsigned char *)calloc(BM_HEADER_SIZE, sizeof(unsigned char));
    memcpy(result->data, BM_MAGIC, 2);
    return result;
}


void bmp_free(bmp_result * result) {
   
}
