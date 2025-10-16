#include "assembler.h"
#include "buffer.h"
#include "Files.h"

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>

#define N_DEBUG 1L
#define MAX_COUNT_ERR 20
#define SIZE_BYTE_CODE 100

void printErrors();

extern asm_error_struct asm_errors[MAX_COUNT_ERR];
extern label_struct labels[10];

int main()
{
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
    if (writeByteCode(byte_code, input_file, &count_elems)) // тут тоже
    {
        printErrors();
        fclose(input_file);
        return 1;
    }
    for (size_t index = 0; index < count_elems; index++)
    {
        printf("byte_code[%zu] = %d\n", index,  byte_code[index]);
    }
    labelsVerify(byte_code, count_elems);

    for (int i = 0; i < 10; i++)
    {
        printf("label[%d] in main = %d\n", i, labels[i].name);
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

void printErrors()
{
    for (int index = 1; index < MAX_COUNT_ERR; index++)
    {
        if (asm_errors[index].status == true)
        {
            fprintf(stderr, "error in %s:%d\n", "Assembler.asm", asm_errors[index].error_lines[index].line);
            fprintf(stderr, "%s\n", asm_errors[index].description);
        }
    }
}
