#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "safetyOfStack.h"
#include "stackFuncs.h"

typedef struct commandStorage
{
    const char*  PUSH   = "PUSH";
    const char*  POP   = "POP";
    const char*  OUT   = "OUT";
    const char*  ADD   = "ADD";
    const char*  SUB   = "SUB";
    const char*  MUL   = "MUL";
    const char*  DIV   = "DIV";
    const char*  SQRT   = "SQRT";
    const char*  HLT   = "HLT";
    const char*  PUSHR = "PUSHR";
    const char*  POPR  = "POPR";
    const char*  JMP   = "JMP";
    const char*  JB    = "JB";
    const char*  JBE   = "JBE";
    const char*  JA    = "JA";
    const char*  JAE   = "JAE";
    const char*  JE    = "JE";
    const char*  JNE   = "JNE";
    const char* CALL   = "CALL";
    const char* RET    = "RET";
    const char* DUMP   = "DUMP";
    const char* PUSHM  = "PUSHM";
    const char* POPM   = "POPM";

    stack_elem_t number = BAD_VALUE;
} commandStorage;

typedef enum
{
    code_NAN   = 0,
    code_PUSH  = 1,
    code_POP   = 2,
    code_OUT   = 3,
    code_ADD   = 4,
    code_SUB   = 5,
    code_MUL   = 6,
    code_DIV   = 7,
    code_SQRT  = 8,
    code_HLT   = 9,
    code_JMP   = 10,
    code_JB    = 11,
    code_JBE   = 12,
    code_JA    = 13,
    code_JAE   = 14,
    code_JE    = 15,
    code_JNE   = 16,
    code_CALL  = 20,
    code_RET   = 21,
    code_DUMP  = 25,
    code_PUSHR = 33,
    code_POPR  = 42,
    code_PUSHM = 62,
    code_POPM  = 63
} code_t;

int_error_t calculatorOn (my_stack_t* stack);
bool getCommand (commandStorage* command, code_t* code_command, stack_elem_t* number,
                                                 char* name_command, FILE* input_file);
#endif // CALCULATOR_H
