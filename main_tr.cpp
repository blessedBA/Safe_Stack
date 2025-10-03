#include "buffer.h"
#include "../Files.h"

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

char* creatBuffer(ssize_t* out_size, const char* filepath);
void  writeBuffer(const char* buffer, FILE* input_file, ssize_t* size);

int main()
{
    const char* filepath = ASSEMBLER;
    FILE* input_file  = fopen(ASSEMBLER, "r");
    FILE* output_file = fopen(BYTE_CODE, "w");
    if (input_file == nullptr || output_file == nullptr)
    {
        fprintf(stderr, "failed to open file\n");
        return 1;
    }
    ssize_t size_file = 0;
    char* buffer = creatBuffer(&size_file, filepath);
    if (buffer == nullptr)
    {
        assert(0 && "failed to allocate memory\n");
    }
    writeBuffer(buffer, input_file, &size_file);
    creatByteCode(buffer, output_file, size_file);
    fclose(input_file);

    return 0;
}


