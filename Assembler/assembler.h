#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "calculator.h"
#include "../Processor/processor_PASH.h"

#include <stdbool.h>

#define MAX_COUNT_ERR 20
#define BAD_VALUE -666
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
    ERR_INV_COM_asm     = 1,
    ERR_FAIL_ALLOC_asm  = 2,
    ERR_INV_LABEL_asm   = 3,
    ERR_MAX_LABELS_asm  = 4,
    ERR_UNEXP_SYM_asm   = 5,
    ERR_FATAL_ERROR_asm = 6
} asm_error_t;


typedef struct label_struct
{
    label_t name = BAD_VALUE;
    label_t adress_label = BAD_VALUE;
    bool isInit = false;
    bool isUsed = false;
} label_struct;

typedef struct func_data_asm
{
    char* file_name;
    char* func_name;
    int line = BAD_VALUE;
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

typedef struct asm_error_struct
{
    asm_error_t code_asm_error;
    func_data_asm error_lines[MAX_COUNT_ERR];
    const char* description;
    bool status;
} asm_error_struct;

asm_error_t creatBuffer(int* byte_code, size_t* size_byte_code);
bool writeByteCode (int* byte_code, FILE* asm_file,
                           size_t* count_elems);

bool checkLabel (help_var_t* variables, FILE* asm_file, size_t* count_elems, int* byte_array, size_t count_lines, size_t* count_labels);
void checkComment (help_var_t* variables, FILE* asm_file, size_t count_lines);

isArg getCodeCommand (help_var_t* variables);
void  getStrArg (help_var_t* variables);

FILE* creatByteFile (int* byte_code, size_t size_byte_code);
void labelsVerify (int* byte_code, size_t count_elems);

asm_error_t scanLabel (FILE* asm_file, size_t* count_elems, int* byte_code);
asm_error_t scanJump (help_var_t* variables, FILE* asm_file, code_t code_command, int* byte_array, size_t* count_elems);
bool        scanComment (FILE* asm_file);

void skipString (FILE* asm_file);
void skipSpaces(FILE* input_file);
void skipString (FILE* asm_file);

void setError (size_t count_line, asm_error_t code_error);
void setCode(help_var_t* variables, int* byte_array, size_t* count_elems);
void setCodeWithArg (help_var_t* variables, int* byte_array, FILE* asm_file, size_t* count_elems, size_t count_lines);
void setRegistr (int* byte_array, help_var_t* variables, size_t* count_elems, FILE* asm_file, code_t command);
void setElemRAM (int* byte_array, help_var_t* variables, size_t* count_elems, FILE* asm_file, code_t command);
void setCodeRAM (int* byte_array, help_var_t* variables, FILE* asm_file, size_t* count_elems, code_t code_RAM_command);

#endif // ASSEMBLER_H
