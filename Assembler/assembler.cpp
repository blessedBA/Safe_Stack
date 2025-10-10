#include "assembler.h"
#include "calculator.h"
#include "Files.h"
#include "generalObjectForStack.h"
#include "processor_PASH.h"
#include "stackFuncs.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define N_DEBUD 1L
#define FIRST_SIZE_B_T 100
#define MAX_COUNT_ERR 20
#define NUMBER_LABELS 10

label_struct labels[NUMBER_LABELS] = {};


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
    help_var_t variables = {};
    while (true)
    {
        // variables.ch = fgetc(asm_file);
        // if (variables.ch == ':')
        // {
        //     scanLabel(asm_file, count_elems); // TODO добавить обработку ошибок
        //     skipString(asm_file);
        //     continue;
        // }
        // ungetc(variables.ch, asm_file);
        fscanf(asm_file, "%5s", &variables.command);
        if ((variables.ch = fgetc(asm_file)) == EOF)
        {
            break;
        }
        switch (getCodeCommand(&variables))
        {
            case HAVE_ERROR:
                variables.flag_error = true;
                if (variables.ind_error + 1 == MAX_COUNT_ERR) return ERR_FATAL_ERROR_asm;
                error_lines[variables.ind_error].file_name = ASSEMBLER; // asm
                error_lines[variables.ind_error].func_name = (char*)__func__;
                error_lines[variables.ind_error].line      = *count_elems;
                variables.ind_error++;
                break;
            case HAVE_NO_ARG:
                switch (variables.code_command)
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
                switch(variables.code_command)
                {
                    case code_PUSH:
                        fscanf(asm_file, "%d\n", &variables.argument);
                        //printf("count_elems [%p]\n", count_elems);
                        byte_array[*count_elems] = code_PUSH;
                        *count_elems += 1;
                        byte_array[*count_elems] = variables.argument;
                        *count_elems += 1;
                        break;
                    case code_PUSHR:
                        fscanf(asm_file, "%s\n", variables.str_arg);
                        #if N_DEBUG
                        assert(variables.str_arg);
                        #endif
                        getStrArg(&variables);
                        byte_array[*count_elems] = code_PUSHR;
                        *count_elems += 1;
                        byte_array[*count_elems] = variables.code_str_arg;
                        *count_elems += 1;
                        break;
                    case code_POPR:
                        fscanf(asm_file, "%s\n", variables.str_arg);
                        #if N_DEBUG
                        assert(variables.str_arg);
                        #endif
                        getStrArg(&variables);
                        byte_array[*count_elems] = code_POPR;
                        *count_elems += 1;
                        byte_array[*count_elems] = variables.code_str_arg;
                        *count_elems += 1;
                        break;
                    case code_JMP:
                        fscanf(asm_file, "%d", &variables.argument);
                        byte_array[*count_elems] = code_JMP;
                        *count_elems += 1;
                        byte_array[*count_elems] = variables.argument;
                        *count_elems += 1;
                        break;
                    case code_JB:
                        fscanf(asm_file, "%d", &variables.argument);
                        byte_array[*count_elems] = code_JB;
                        *count_elems += 1;
                        byte_array[*count_elems] = variables.argument;
                        *count_elems += 1;
                        break;
                    case code_JBE:
                        fscanf(asm_file, "%d", &variables.argument);
                        byte_array[*count_elems] = code_JBE;
                        *count_elems += 1;
                        byte_array[*count_elems] = variables.argument;
                        *count_elems += 1;
                        break;
                    case code_JA:
                        fscanf(asm_file, "%d", &variables.argument);
                        byte_array[*count_elems] = code_JA;
                        *count_elems += 1;
                        byte_array[*count_elems] = variables.argument;
                        *count_elems += 1;
                        break;
                    case code_JAE:
                        fscanf(asm_file, "%d", &variables.argument);
                        byte_array[*count_elems] = code_JAE;
                        *count_elems += 1;
                        byte_array[*count_elems] = variables.argument;
                        *count_elems += 1;
                        break;
                    case code_JE:
                        fscanf(asm_file, "%d", &variables.argument);
                        byte_array[*count_elems] = code_JE;
                        *count_elems += 1;
                        byte_array[*count_elems] = variables.argument;
                        *count_elems += 1;
                        break;
                    case code_JNE:
                        fscanf(asm_file, "%d", &variables.argument);
                        byte_array[*count_elems] = code_JNE;
                        *count_elems += 1;
                        byte_array[*count_elems] = variables.argument;
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

    return (variables.flag_error) ? ERR_INV_COM_asm : HAVE_NO_ERRORS_asm;
}

isArg getCodeCommand (help_var_t* variables)
{
    //printf("name_command [%p]\n", name_command);
    if (strcmp(variables->commands.PUSH, variables->command) == 0)
    {
        variables->code_command = code_PUSH;
        return HAVE_ARG;
    }
    else if (strcmp(variables->commands.PUSHR, variables->command) == 0)
    {
        variables->code_command = code_PUSHR;
        return HAVE_ARG;
    }
    else if (strcmp(variables->commands.POPR, variables->command) == 0)
    {
        variables->code_command = code_POPR;
        return HAVE_ARG;
    }
    else if (strcmp(variables->commands.JMP, variables->command) == 0)
    {
        variables->code_command = code_JMP;
        return HAVE_ARG;
    }
    else if (strcmp(variables->commands.JB, variables->command) == 0)
    {
        variables->code_command = code_JB;
        return HAVE_ARG;
    }
    else if (strcmp(variables->commands.JBE, variables->command) == 0)
    {
        variables->code_command = code_JBE;
        return HAVE_ARG;
    }
    else if (strcmp(variables->commands.JA, variables->command) == 0)
    {
        variables->code_command = code_JA;
        return HAVE_ARG;
    }
    else if (strcmp(variables->commands.JAE, variables->command) == 0)
    {
        variables->code_command = code_JAE;
        return HAVE_ARG;
    }
    else if (strcmp(variables->commands.JE, variables->command) == 0)
    {
        variables->code_command = code_JE;
        return HAVE_ARG;
    }
    else if (strcmp(variables->commands.JNE, variables->command) == 0)
    {
        variables->code_command = code_JNE;
        return HAVE_ARG;
    }
    else if (strcmp(variables->commands.POP, variables->command) == 0)
    {
        variables->code_command = code_POP;
        return HAVE_NO_ARG;
    }
    else if (strcmp(variables->commands.OUT, variables->command) == 0)
    {
        variables->code_command = code_OUT;
        return HAVE_NO_ARG;
    }
    else if (strcmp(variables->commands.ADD, variables->command) == 0)
    {
        variables->code_command = code_ADD;
        return HAVE_NO_ARG;
    }
    else if (strcmp(variables->commands.SUB, variables->command) == 0)
    {
        variables->code_command = code_SUB;
        return HAVE_NO_ARG;
    }
    else if (strcmp(variables->commands.MUL, variables->command) == 0)
    {
        variables->code_command = code_MUL;
        return HAVE_NO_ARG;
    }
    else if (strcmp(variables->commands.DIV, variables->command) == 0)
    {
        variables->code_command = code_DIV;
        return HAVE_NO_ARG;
    }
    else if (strcmp(variables->commands.SQRT, variables->command) == 0)
    {
        variables->code_command = code_SQRT;
        return HAVE_NO_ARG;
    }
    else if (strcmp(variables->commands.HLT, variables->command) == 0)
    {
        variables->code_command = code_HLT;
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

void getStrArg (help_var_t* variables)
{
    #if N_DEBUG
    assert(variables);
    #endif
    if (strcmp(variables->arguments.RAX, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_RAX;
    }
    else if (strcmp(variables->arguments.RBX, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_RBX;
    }
    else if (strcmp(variables->arguments.RCX, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_RCX;
    }
    else if (strcmp(variables->arguments.RDX, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_RDX;
    }
    else if (strcmp(variables->arguments.REX, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_REX;
    }
    else if (strcmp(variables->arguments.RFX, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_RFX;
    }
    else if (strcmp(variables->arguments.RGX, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_RGX;
    }
    else if (strcmp(variables->arguments.RHX, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_RHX;
    }

    return;
}

asm_error_t scanLabel (FILE* asm_file, int* count_elems)
{
    label_t curr_label = 0;
    char ch = 0;
    bool isReadLabel = false;
    if (fscanf(asm_file, "%d", &curr_label) != 1 || (ch = fgetc(asm_file)) != '\n')
    {
        return ERR_INV_LABEL_asm;
    }
    for (int index = 0; index < NUMBER_LABELS; index++)
    {
        if (labels[index].status == false)
        {
            isReadLabel = true;
            labels[index].name = curr_label;
            labels[index].adress_label = *count_elems;
            *count_elems += 1;
        }
    }
    return (isReadLabel) ? HAVE_NO_ERRORS_asm : ERR_MAX_LABELS_asm;
}

void skipString (FILE* asm_file)
{
    #if N_DEBUG
    assert(asm_file);
    #endif

    char ch = 0;
    while((ch = fgetc(asm_file)) != '\n')
    ;

    return;
}
