#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "calculator.h"
#include "processor_PASH.h"

typedef enum
{
    HAVE_ERROR = -1,
    HAVE_NO_ARG = 0,
    HAVE_ARG = 1
} isArg;

typedef enum
{
    HAVE_NO_ERRORS_asm  = 0,
    ERR_FATAL_ERROR_asm = 1,
    ERR_INV_COM_asm     = 2,
    ERR_FAIL_ALLOC_asm  = 3
} asm_error_t;

typedef struct
{
    char* file_name;
    char* func_name;
    int line;
} func_data_asm;

asm_error_t creatByteCode(int* byte_code, size_t* size_byte_code);
asm_error_t writeByteCode (int* byte_code, FILE* asm_file,
                           size_t* count_elems, func_data_asm* error_lines);
isArg getCodeCommand (commandStorage* command, char* name_command, code_t* code_command);
FILE* creatByteFile (int* byte_code, size_t size_byte_code);
void getStrArg (argumentStorage* arguments, char* str_arg, str_arg_t* code_str_arg);
#endif // ASSEMBLER_H
