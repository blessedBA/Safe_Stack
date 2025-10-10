#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "calculator.h"
#include "processor_PASH.h"

typedef int label_t;

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
    ERR_FAIL_ALLOC_asm  = 3,
    ERR_INV_LABEL_asm   = 4,
    ERR_MAX_LABELS_asm  = 5
} asm_error_t;

typedef struct func_data_asm
{
    char* file_name;
    char* func_name;
    int line;
} func_data_asm;

typedef struct help_var_t
{
    int ch;
    commandStorage commands;
    char command[6];
    code_t code_command = code_NAN;
    int ind_error = 0;
    bool flag_error = false;
    char str_arg[5];
    str_arg_t code_str_arg = code_NaN;
    registrStrStorage arguments = {};
    stack_elem_t argument = BAD_VALUE;
    bool isLabel = false;
} help_var_t;

typedef struct label_struct
{
    label_t name = BAD_VALUE;
    label_t adress_label = BAD_VALUE;
    bool status = false;
} label_struct;

asm_error_t creatByteCode(int* byte_code, size_t* size_byte_code);
asm_error_t writeByteCode (int* byte_code, FILE* asm_file,
                           size_t* count_elems, func_data_asm* error_lines);
isArg getCodeCommand (help_var_t* variables);
FILE* creatByteFile (int* byte_code, size_t size_byte_code);
void getStrArg (help_var_t* variables);
void skipString (FILE* asm_file);
#endif // ASSEMBLER_H
