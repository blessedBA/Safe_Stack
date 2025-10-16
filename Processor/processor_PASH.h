#ifndef PROCESSOR_PASH_H
#define PROCESSOR_PASH_H

#include "errors.h"
#include "generalObjectForStack.h"
#include "RAM.h"
#include "stackFuncs.h"

#include <stdio.h>

#define NUMBER_REGISTERS 8

typedef int byte_code_t;
typedef long long register_t;
typedef struct reg_struct
{
    register_t RAX = BAD_VALUE;
    register_t RBX = BAD_VALUE;
    register_t RCX = BAD_VALUE;
    register_t RDX = BAD_VALUE;
    register_t REX = BAD_VALUE;
    register_t RFX = BAD_VALUE;
    register_t RGX = BAD_VALUE;
    register_t RHX = BAD_VALUE;
} reg_struct;

typedef struct registrStrStorage
{
    const char* RAX = "RAX";
    const char* RBX = "RBX";
    const char* RCX = "RCX";
    const char* RDX = "RDX";
    const char* REX = "REX";
    const char* RFX = "RFX";
    const char* RGX = "RGX";
    const char* RHX = "RHX";
} registrStrStorage;

typedef enum
{
    code_NaN = -1,
    code_RAX =  555,
    code_RBX =  556,
    code_RCX =  557,
    code_RDX =  558,
    code_REX =  559,
    code_RFX =  560,
    code_RGX =  561,
    code_RHX =  562
} str_arg_t;


typedef struct
{
    byte_code_t* b_code;
    size_t      count_elems;
} b_code_struct;

typedef struct
{
    my_stack_t stack1;
    my_stack_t stackCall;
    ram_t*     RAM;
    size_t     sizeRAM;
    b_code_struct byte_code;
    size_t P_C;
    size_t curr_command;
    reg_struct registers;
    error_pr_t curr_error;
} Processor_t;


size_t checkCountElems(FILE* byte_file);
int_error_t readByteFile(Processor_t* processor, FILE* byte_file);
int_error_t doByteCode (Processor_t* processor);
int_error_t procVerify (Processor_t* processor, const char* file_name,
                                                const char* func_name,
                                                int line);
void writeToRegistr (Processor_t* processor, int code_registr);
void getValue (Processor_t* processor, int code_registr);
void procDump (Processor_t* processor, int global_code_error, func_data* f_data_pr);
#endif // PROCESSOR_PASH_H
