#include "assembler.h"
#include "calculator.h"
#include "Files.h"
#include "generalObjectForStack.h"
#include "processor_PASH.h"
#include "stackFuncs.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define N_DEBUD 1L
#define FIRST_SIZE_B_T 100
#define MAX_COUNT_ERR 20

asm_error_t creatByteCode(int* byte_code, size_t* size_byte_code)
{
    *size_byte_code = FIRST_SIZE_B_T;
    byte_code = (int*)calloc(FIRST_SIZE_B_T, sizeof(int));
    #if N_DEBUG
    if (byte_code == nullptr || size_byte_code == nullptr)
    {
        return ERR_FAIL_ALLOC_asm;
    }
    #endif
    return HAVE_NO_ERRORS_asm;
}
asm_error_t writeByteCode (int* byte_array, FILE* asm_file,
                           size_t* count_elems, func_data_asm* error_lines)
{
    #if N_DEBUD
    assert(byte_array  != nullptr &&
           asm_file    != nullptr &&
           error_lines != nullptr &&
           count_elems != nullptr    );
    #endif
    commandStorage commands = {};
    argumentStorage arguments = {};
    char command[6] = {};
    char ch = 0;
    int ind_error = 0;
    stack_elem_t argument = BAD_VALUE;
    bool flag_error = false;
    code_t code_command = code_NAN;
    str_arg_t code_str_arg = code_NaN;
    char str_arg[5] = "";
    while (true)
    {
        fscanf(asm_file, "%s", &command);
        if ((ch = fgetc(asm_file)) == EOF)
        {
            break;
        }
        switch (getCodeCommand(&commands, command, &code_command))
        {
            case HAVE_ERROR:
                flag_error = true;
                if (ind_error + 1 == MAX_COUNT_ERR) return ERR_FATAL_ERROR_asm;
                error_lines[ind_error].file_name = __FILE__;
                error_lines[ind_error].func_name = (char*)__func__;
                error_lines[ind_error].line      = *count_elems;
                ind_error++;
                break;
            case HAVE_NO_ARG:
                switch (code_command)
                {
                    case code_POP:
                    byte_array[*count_elems] = code_POP;
                    *count_elems += 1;
                        break;
                    case code_OUT:
                    byte_array[*count_elems] = code_OUT;
                    *count_elems += 1;
                        break;
                    case code_ADD:
                    byte_array[*count_elems] = code_ADD;
                    *count_elems += 1;
                        break;
                    case code_SUB:
                    byte_array[*count_elems] = code_SUB;
                    *count_elems += 1;
                        break;
                    case code_MUL:
                    byte_array[*count_elems] = code_MUL;
                    *count_elems += 1;
                        break;
                    case code_DIV: // TODO добавить проверку на деление на ноль
                    byte_array[*count_elems] = code_DIV;
                    *count_elems += 1;
                        break;
                    case code_SQRT:
                    byte_array[*count_elems] = code_SQRT;
                    *count_elems += 1;
                        break;
                    case code_HLT:
                    byte_array[*count_elems] = code_HLT;
                    *count_elems += 1;
                        break;
                    default:
                        assert(0 && "invalid code_command\n");
                }
                break;
            case HAVE_ARG:
                switch(code_command)
                {
                    case code_PUSH:
                        fscanf(asm_file, "%d\n", &argument);
                        //printf("count_elems [%p]\n", count_elems);
                        byte_array[*count_elems] = code_PUSH;
                        *count_elems += 1;
                        byte_array[*count_elems] = argument;
                        *count_elems += 1;
                        break;
                    case code_PUSHR:
                        fscanf(asm_file, "%s\n", str_arg);
                        #if N_DEBUG
                        assert(str_arg);
                        #endif
                        getStrArg(&arguments, str_arg, &code_str_arg);
                        byte_array[*count_elems] = code_PUSHR;
                        *count_elems += 1;
                        byte_array[*count_elems] = code_str_arg;
                        *count_elems += 1;
                        break;
                    case code_POPR:
                        fscanf(asm_file, "%s\n", str_arg);
                        #if N_DEBUG
                        assert(str_arg);
                        #endif
                        getStrArg(&arguments, str_arg, &code_str_arg);
                        byte_array[*count_elems] = code_POPR;
                        *count_elems += 1;
                        byte_array[*count_elems] = code_str_arg;
                        *count_elems += 1;
                        break;
                    default:
                        fprintf(stderr, "invalid command, try again\n");
                        break;
                }
                break;
        }
    }
    byte_array[0] = *count_elems;

    return (flag_error) ? ERR_INV_COM_asm : HAVE_NO_ERRORS_asm;
}

isArg getCodeCommand (commandStorage* command, char* name_command, code_t* code_command)
{
    // printf("name_command [%p]\n", name_command);
    if (strcmp(command->PUSH, name_command) == 0)
    {
        *code_command = code_PUSH;
        return HAVE_ARG;
    }
    else if (strcmp(command->PUSHR, name_command) == 0)
    {
        *code_command = code_PUSHR;
        return HAVE_ARG;
    }
    else if (strcmp(command->POPR, name_command) == 0)
    {
        *code_command = code_POPR;
        return HAVE_ARG;
    }
    else if (strcmp(command->POP, name_command) == 0)
    {
        *code_command = code_POP;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->OUT, name_command) == 0)
    {
        *code_command = code_OUT;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->ADD, name_command) == 0)
    {
        *code_command = code_ADD;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->SUB, name_command) == 0)
    {
        *code_command = code_SUB;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->MUL, name_command) == 0)
    {
        *code_command = code_MUL;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->DIV, name_command) == 0)
    {
        *code_command = code_DIV;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->SQRT, name_command) == 0)
    {
        *code_command = code_SQRT;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->HLT, name_command) == 0)
    {
        *code_command = code_HLT;
        return HAVE_NO_ARG;
    }
    else
    {
        return HAVE_ERROR;
    }

}

FILE* creatByteFile (int* byte_code, size_t count_elems)
{
    #if N_DEBUG
    assert(byte_code && count_elems > 0);
    #endif
    FILE* byte_file = fopen(BYTE_CODE, "wb");
    #if N_DEBUG
    assert(byte_file);
    #endif
    fwrite(byte_code, count_elems, sizeof(int), byte_file);

    return byte_file;
}

void getStrArg (argumentStorage* arguments, char* str_arg, str_arg_t* code_str_arg)
{
    #if N_DEBUG
    assert(arguments && str_arg && code_str_arg);
    #endif
    if (strcmp(arguments->RAX, str_arg) == 0)
    {
        *code_str_arg = code_RAX;
    }
    else if (strcmp(arguments->RBX, str_arg) == 0)
    {
        *code_str_arg = code_RBX;
    }
    else if (strcmp(arguments->RCX, str_arg) == 0)
    {
        *code_str_arg = code_RCX;
    }
    else if (strcmp(arguments->RDX, str_arg) == 0)
    {
        *code_str_arg = code_RDX;
    }
    else if (strcmp(arguments->REX, str_arg) == 0)
    {
        *code_str_arg = code_REX;
    }
    else if (strcmp(arguments->RFX, str_arg) == 0)
    {
        *code_str_arg = code_RFX;
    }
    else if (strcmp(arguments->RGX, str_arg) == 0)
    {
        *code_str_arg = code_RGX;
    }
    else if (strcmp(arguments->RHX, str_arg) == 0)
    {
        *code_str_arg = code_RHX;
    }

    return;
}
