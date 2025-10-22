#include "../Assembler/assembler.h"
#include "Files.h"
#include "errors.h"
#include "processor_PASH.h"
#include "RAM.h"
#include "stackFuncs.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

//#define NDEBUG
#ifndef N_DEBUG 1L
#define N_DEBUG 1L
#endif

void dumpRAM (Processor_t* processor);

int main()
{
    Processor_t* processor = creatProcessor();
    //printf("ptr to ram in main = %p\n", processor->RAM);
    //printf("sizeRAM in main = %zu\n", processor->sizeRAM);
    #ifndef NDEBUG
    if (processor->RAM == nullptr)
    {
        fprintf(stderr, "failed to create ram\n");
        return 1;
    }
    #endif
    //char cwd[PATH_MAX] = "";
    //fprintf(stderr, "DEBUG: cwd='%s' trying to open '%s'\n", cwd, BYTE_CODE_FOR_PROC);
    FILE* byte_file = fopen(BYTE_CODE_FOR_PROC, "rb");
    #ifndef NDEBUG
    if (byte_file == nullptr)
    {
        fprintf(stderr, "failed to open byte code\n");
        return 1;
    }
    #endif
    processor->byte_code.count_elems = checkCountElems(byte_file);
    //printf("processor.byte_code.count_elems = %d\n", processor.byte_code.count_elems);
    #ifndef NDEBUG
    if (processor->byte_code.count_elems == 0)
    {
        fprintf(stderr, "invalid count_elems\n");
        return 1;
    }
    #endif
    processor->byte_code.b_code = (byte_code_t*)calloc((size_t)processor->byte_code.count_elems, sizeof(byte_code_t));
    #ifndef NDEBUG
    printf("IN %s: processor->byte_code.count_elems = %zu\n", __func__, processor->byte_code.count_elems);
    printf("IN %s: processor [%p]\n", __func__, processor);
    printf("IN %s: processor->byte_code.b_code [%p]\n", __func__, processor->byte_code.b_code);
    printf("IN %s: byte_file [%p]\n", __func__, byte_file);
    printf("IN %s: processor->byte_code.b_code[0] = %d\n", __func__, processor->byte_code.b_code[0]);
    if (processor->byte_code.b_code == nullptr)
    {
        fprintf(stderr, "failed to allocate memory\n");
        return 1;
    }
    if (readByteFile(processor, byte_file) == ERR_READ_BT)
    {
        fprintf(stderr, "failed to read byte code\n");
        return 1;
    }
    if (fclose(byte_file) != 0)
    {
        fprintf(stderr, "failed to close byte file\n");
    }
    #endif
    for (int index = 0; index < processor->byte_code.count_elems; index++)
    {
        printf("byte_code[%d] = %d\n", index, processor->byte_code.b_code[index]);

    }
    //printf("byte file:::\t\t");
    //printf("\n");

    doByteCode(processor);
    dumpRAM(processor);
    //printStack(&processor.stack1, stdout);
    stackDestroy(&(processor->stack1));
    stackDestroy(&(processor->stackCall));
    free(processor->byte_code.b_code);
    free(processor->RAM);
    free(processor);
    return 0;
}






