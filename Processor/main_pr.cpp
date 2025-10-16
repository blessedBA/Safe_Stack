#include "../Assembler/assembler.h"
#include "Files.h"
#include "errors.h"
#include "processor_PASH.h"
#include "RAM.h"
#include "stackFuncs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#define SIZE_RAM 100
#ifndef N_DEBUG 1L
#define N_DEBUG 1L
#endif

int main()
{
    Processor_t processor = { .stack1 = {}, .stackCall = {},
                              .RAM = nullptr, .sizeRAM = -1zu,
                              .byte_code.b_code = nullptr, .byte_code.count_elems = 0,
                              .P_C = 0, .curr_command = 0, .curr_error = NO_ERRORS_PR,
                              .registers = {} };
    processor.sizeRAM = SIZE_RAM;
    ram_t* RAM = createRAM(processor.sizeRAM);
    processor.RAM = RAM;
    #if N_DEBUG
    if (RAM == nullptr)
    {
        fprintf(stderr, "failed to create ram\n");
        return 1;
    }
    #endif
    //char cwd[PATH_MAX] = "";
    //fprintf(stderr, "DEBUG: cwd='%s' trying to open '%s'\n", cwd, BYTE_CODE_FOR_PROC);
    FILE* byte_file = fopen(BYTE_CODE_FOR_PROC, "rb");
    #if N_DEBUG
    if (byte_file == nullptr)
    {
        fprintf(stderr, "failed to open byte code\n");
        return 1;
    }
    #endif
    processor.byte_code.count_elems = checkCountElems(byte_file);
    //printf("processor.byte_code.count_elems = %d\n", processor.byte_code.count_elems);
    #if N_DEBUG
    if (processor.byte_code.count_elems == 0)
    {
        fprintf(stderr, "invalid count_elems\n");
        return 1;
    }
    #endif
    processor.byte_code.b_code = (byte_code_t*)calloc((size_t)processor.byte_code.count_elems, sizeof(byte_code_t));
    #if N_DEBUG
    if (processor.byte_code.b_code == nullptr)
    {
        fprintf(stderr, "failed to allocate memory\n");
        return 1;
    }
    if (readByteFile(&processor, byte_file) == ERR_READ_BT)
    {
        fprintf(stderr, "failed to read byte code\n");
        return 1;
    }
    if (fclose(byte_file) != 0)
    {
        fprintf(stderr, "failed to close byte file\n");
    }
    #endif
    for (int index = 0; index < processor.byte_code.count_elems; index++)
    {
        printf("byte_code[%d] = %d\n", index, processor.byte_code.b_code[index]);
    }
    //printf("byte file:::\t\t");
    //printf("\n");
    stackInit(&(processor.stack1));
    stackInit(&(processor.stackCall));

    doByteCode(&processor);
    //printStack(&processor.stack1, stdout);
    stackDestroy(&(processor.stack1));
    stackDestroy(&(processor.stackCall));
    //printf("RAX = %lld\n", processor.registers.RAX);
    //printf("RBX = %lld\n", processor.registers.RBX);
    free(processor.byte_code.b_code);
    return 0;
}






