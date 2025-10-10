#include "assembler.h"
#include "buffer.h"
#include "Files.h"

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#define N_DEBUG 1L
#define MAX_COUNT_ERR 20
#define SIZE_BYTE_CODE 100

int main()
{
    func_data_asm error_lines[MAX_COUNT_ERR] = {};
    FILE* input_file  = fopen(ASSEMBLER, "r");

    if (input_file == nullptr)
    {
        fprintf(stderr, "failed to open file\n");
        return 1;
    }

    int byte_code[SIZE_BYTE_CODE] = {};
    size_t size_byte_code  = 100;
    size_t count_elems     = 1; // тк первое число под колво элементов
    if (creatByteCode(byte_code, &size_byte_code)) // стоит дальше развивать логику обработки ошибок
    {
        fprintf(stderr, "something with creatByteCode went wrong...\n");
        fclose(input_file);
        return 1;
    }
    printf("byte_code [%p]\ninput_file [%p]\ncount_elems [%p]\nerror_lines [%p]\n", byte_code, input_file, &count_elems, error_lines);
    if (writeByteCode(byte_code, input_file, &count_elems, error_lines)) // тут тоже
    {
        fprintf(stderr, "something with writeByteCode went wrong...\n");
        fclose(input_file);
        return 1;
    }
    FILE* byte_file = creatByteFile(byte_code, count_elems);
    if (byte_file == nullptr)
    {
        fprintf(stderr, "failed to open byte_file \n");
        return 1;
    }

    fclose(byte_file);
    fclose(input_file);

    return 0;
}
