#include <stdint.h>

#ifndef HEADER_BMP_LIB
#define HEADER_BMP_LIB

struct _bmp_result_holder {
    int32_t dataSize;
    unsigned char * data;
};

typedef struct _bmp_result_holder bmp_result; 

bmp_result * create_bw_bmp(const unsigned char * data, const uint32_t dataLen, const uint32_t maxWidth);

void bmp_free(bmp_result * result);

#endif
