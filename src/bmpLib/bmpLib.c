#include <stdlib.h>

#include "bmpLib.h"

bmp_result * create_bw_bmp(const unsigned char * data, const unsigned int dataLen) {
    bmp_result * result = (bmp_result *)calloc(1, sizeof(bmp_result));
    result->dataSize = 0;
    result->data = (unsigned char *)calloc(0, sizeof(unsigned char));
    return result;
}


void bmp_free(bmp_result * result) {
   
}
