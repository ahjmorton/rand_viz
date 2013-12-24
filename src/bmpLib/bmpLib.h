#ifndef HEADER_BMP_LIB
#define HEADER_BMP_LIB

struct _bmp_result_holder {
    int resultSize;
    unsigned char * result;
};

typedef struct _bmp_result_holder bmp_result; 

bmp_result * create_bw_bmp(const unsigned char * data, const unsigned int dataLen);

void bmp_free(bmp_result * result);

#endif
