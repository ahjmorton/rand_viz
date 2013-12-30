#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include "bmpLib.h"

#define BUFFER_INC 256

static uint32_t read_in_input(FILE* input, unsigned char ** result);

static void write_out_bmp(const bmp_result * const output, FILE* input);

int main(void) {
    unsigned char * input = NULL;
    const uint32_t bytesRead = read_in_input(stdin, &input);
    bmp_result * output = create_bw_bmp(input, bytesRead, 256);
    free(input);  
    write_out_bmp(output, stdout);
    bmp_free(output);
}

static uint32_t read_in_input(FILE* stream, unsigned char ** result) {
    uint32_t count = 0;
    const size_t charSize = sizeof(unsigned char);
    unsigned char * buffer = (unsigned char *) malloc(BUFFER_INC * charSize);
    int reading = 1;
    while(reading) {
        size_t read = fread(buffer + count, charSize, BUFFER_INC, stream);
        count += read; 
        if(read == 0) {
            if(feof(stream)) {
                reading = 0;
            }
            else if(ferror(stream)) {
                return 0;
            }
        } else if (read == BUFFER_INC) {
            buffer = (unsigned char *)realloc(buffer, (count + BUFFER_INC) * charSize);
        }
    } 
    buffer = (unsigned char *)realloc(buffer, count * charSize);
    (*result) = buffer;
    return count;
}

static void write_out_bmp(const bmp_result * const output, FILE* stream) {
    fwrite(output->data, sizeof(unsigned char), output->dataSize, stream);
}
