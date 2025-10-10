#include "Files.h"
#include "errors.h"
#include "processor_PASH.h"
#include "stackFuncs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

int main()
{
    Processor_t processor = { .stack1 = {}, .byte_code.b_code = nullptr,
                              .byte_code.count_elems = 0,
                              .P_C = 0, .curr_command = 0, .curr_error = NO_ERRORS_PR,
                              .registers = {} };

    //char cwd[PATH_MAX] = "";
    //fprintf(stderr, "DEBUG: cwd='%s' trying to open '%s'\n", cwd, BYTE_CODE_FOR_PROC);
    FILE* byte_file = fopen(BYTE_CODE_FOR_PROC, "rb");
    if (byte_file == nullptr)
    {
        fprintf(stderr, "failed to open byte code\n");
        return 1;
    }
    processor.byte_code.count_elems = checkCountElems(byte_file);
    //printf("processor.byte_code.count_elems = %d\n", processor.byte_code.count_elems);
    if (processor.byte_code.count_elems == 0)
    {
        fprintf(stderr, "invalid count_elems\n");
        return 1;
    }
    processor.P_C++;
    processor.byte_code.b_code = (byte_code_t*)calloc((size_t)processor.byte_code.count_elems, sizeof(byte_code_t));
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
    //printf("byte file:::\t\t");
    //printf("\n");
    // processor.stack1 = {};
    stackInit(&(processor.stack1));

    doByteCode(&processor);
    //printStack(&processor.stack1, stdout);
    stackDestroy(&processor.stack1);
    printf("RAX = %lld\n", processor.registers.RAX);
    printf("RBX = %lld\n", processor.registers.RBX);
    free(processor.byte_code.b_code);
    return 0;
}






