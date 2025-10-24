#include "assembler.h"
#include "calculator.h"
#include "Files.h"
#include "generalObjectForStack.h"
#include "labels_comments.h"
#include "../Processor/processor_PASH.h"
#include "stackFuncs.h"
#include "Utils_asm.h"

#include <assert.h>
#include <ctype.h>
#include <mm_malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#define NDEBUG
#define N_DEBUD 1L
#define FIRST_SIZE_B_T 300
#define MAX_COUNT_ERR 20
#define NUMBER_LABELS 10

#define SET_CODE(command) byte_array[*count_elems] = code_##command; \
                          *count_elems += 1; \
                          break;
#define SET_JUMP(type_jump) if (scanJump(variables, asm_file, code_##type_jump, byte_array, count_elems)) setError(count_lines, ERR_INV_LABEL_asm); \
                            break;
#define CMP_CMD(CMD, isArg) if (strcmp(variables->commands.CMD, variables->command) == 0) { \
                            variables->code_command = code_##CMD; \
                            return isArg; }

label_struct labels[NUMBER_LABELS] = {};
asm_error_struct asm_errors[MAX_COUNT_ERR] = { {.code_asm_error = HAVE_NO_ERRORS_asm, .error_lines = {}, .description = "you have no error!!!",        .status = true },
                                               {.code_asm_error = ERR_INV_COM_asm,    .error_lines = {}, .description = "you entered invalid command", .status = false},
                                               {.code_asm_error = ERR_FAIL_ALLOC_asm, .error_lines = {}, .description = "failed to allocate memory",   .status = false},
                                               {.code_asm_error = ERR_INV_LABEL_asm,  .error_lines = {}, .description = "you entered invalid label",   .status = false},
                                               {.code_asm_error = ERR_MAX_LABELS_asm, .error_lines = {}, .description = "have no free labels",         .status = false},
                                               {.code_asm_error = ERR_UNEXP_SYM_asm,  .error_lines = {}, .description = "unexpected symbol!!",         .status = false} };

asm_error_t creatBuffer(int* byte_code, size_t* size_byte_code)
{
    #ifndef NDEBUG
    assert(byte_code && size_byte_code);
    #endif

    byte_code = (int*)calloc(FIRST_SIZE_B_T, sizeof(int));

    #ifndef NDEBUG
    assert(byte_code && size_byte_code);
    #endif

    return HAVE_NO_ERRORS_asm;
}

void resizeByteCode (int* byte_code, size_t size)
{
    #ifndef NDEBUG
    assert(byte_code);
    #endif

    byte_code = (int*)realloc(byte_code, sizeof(int) * size);

    #ifndef NDEBUG
    assert(byte_code);
    #endif

    return;
}

bool writeByteCode (int* byte_array, FILE* asm_file, size_t* count_elems)
{
    #ifndef NDEBUG
    assert(byte_array != nullptr && asm_file != nullptr && count_elems != nullptr);
    #endif

    help_var_t variables = {};
    size_t count_args   = 0;
    size_t count_lines  = 0;
    size_t count_labels = 0;
    while (true)
    {
        if (*count_elems >= 300)
        {
            resizeByteCode(byte_array, *count_elems);
        }
        // printf("\n\n");
        // printf("line %d\n", count_lines);
        // variables.ch = fgetc(asm_file);
        // if (variables.ch == ';')
        // {
        //     printf("СТРОКА С КОММЕНТОМ\n");
        // }
        // printf("first char of the each string is %c\n", variables.ch);
        // ungetc(variables.ch, asm_file);
        // if (variables.ind_error + 1 == MAX_COUNT_ERR) return true;
        // skipSpaces(asm_file);
        // if (checkLabel(&variables, asm_file, count_elems, byte_array, count_lines, &count_labels))
        // {
        //     continue;
        // }
        // if ((variables.ch = fgetc(asm_file)) == ';')
        // {
        //     count_lines++;
        //     skipString(asm_file);
        //     continue;
        // }
        // printf("variables.ch before main ungetc = %c\n", variables.ch);
        // int ch2 = 0;
        // ch2 = fgetc(asm_file);
        // printf("second char after variables.ch is %c\n", ch2);
        // ungetc(ch2, asm_file);
        // ungetc(variables.ch, asm_file);
        // count_lines = *count_elems + count_labels - count_args;
        // if (variables.ch == EOF) // check EOF
        // {
        //     break;
        // }
        int ch = fgetc(asm_file);
        if (ch == EOF) break;
        ungetc(ch, asm_file);

        skipSpaces(asm_file);

        ch = fgetc(asm_file);
        if (ch == EOF) break;

        if (ch == ';')
        {
            skipString(asm_file);
            count_lines++;
            continue;
        }
        ungetc(ch, asm_file);

        if (checkLabel(&variables, asm_file, count_elems, byte_array, count_lines, &count_labels))
        {
            continue;
        }
        fscanf(asm_file, "%5s", variables.command);
        switch (getCodeCommand(&variables))
        {
            case HAVE_ERROR:
                variables.flag_error = true;
                setError(count_lines, ERR_INV_COM_asm);
                variables.ind_error++;
                if (variables.ind_error + 1 == MAX_COUNT_ERR) return true;
                break;
            case HAVE_NO_ARG:
                setCode(&variables, byte_array, count_elems);
                break;
            case HAVE_ARG:
                count_args++;
                setCodeWithArg(&variables, byte_array, asm_file, count_elems, count_lines);
                break;
            default:
                fprintf(stderr, "эммммм, default в %s %s:%d\n", __FILE__, __func__, __LINE__);
        }
        checkComment(&variables, asm_file, count_lines);
    }

    byte_array[0] = (int)*count_elems;

    #ifndef NDEBUG
    assert(byte_array != nullptr && asm_file != nullptr && count_elems != nullptr);
    #endif

    return variables.flag_error;
}

isArg getCodeCommand (help_var_t* variables)
{
    #ifndef NDEBUG
    assert(variables);
    #endif

    CMP_CMD(PUSH,    HAVE_ARG)
    CMP_CMD(PUSHR,   HAVE_ARG)
    CMP_CMD(POPR,    HAVE_ARG)
    CMP_CMD(JMP,     HAVE_ARG)
    CMP_CMD(JB,      HAVE_ARG)
    CMP_CMD(JBE,     HAVE_ARG)
    CMP_CMD(JA,      HAVE_ARG)
    CMP_CMD(JAE,     HAVE_ARG)
    CMP_CMD(JE,      HAVE_ARG)
    CMP_CMD(JNE,     HAVE_ARG)
    CMP_CMD(CALL,    HAVE_ARG)
    CMP_CMD(PUSHM,   HAVE_ARG)
    CMP_CMD(POPM,    HAVE_ARG)
    CMP_CMD(RET,  HAVE_NO_ARG)
    CMP_CMD(POP,  HAVE_NO_ARG)
    CMP_CMD(OUT,  HAVE_NO_ARG)
    CMP_CMD(ADD,  HAVE_NO_ARG)
    CMP_CMD(SUB,  HAVE_NO_ARG)
    CMP_CMD(MUL,  HAVE_NO_ARG)
    CMP_CMD(DIV,  HAVE_NO_ARG)
    CMP_CMD(SQRT, HAVE_NO_ARG)
    CMP_CMD(HLT,  HAVE_NO_ARG)
    CMP_CMD(DUMP, HAVE_NO_ARG)

    #ifndef NDEBUG
    assert(variables);
    #endif

    return HAVE_ERROR;
}

FILE* creatByteFile (int* byte_code, size_t count_elems)
{
    #ifndef NDEBUG
    assert(byte_code && count_elems > 0);
    #endif

    FILE* byte_file = fopen(BYTE_CODE, "wb");

    #ifndef NDEBUG
    assert(byte_file);
    #endif

    fwrite(byte_code, count_elems, sizeof(int), byte_file);

    return byte_file;
}

void getStrArg (help_var_t* variables)
{
    #ifndef NDEBUG
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
    else if (strcmp(variables->arguments.PUSHM, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_PUSHM_pr;
    }
    else if (strcmp(variables->arguments.POPM, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_POPM_pr;
    }

    #ifndef NDEBUG
    assert(variables);
    #endif

    return;
}

asm_error_t scanJump (help_var_t* variables, FILE* asm_file, code_t code_command, int* byte_array, size_t* count_elems)
{
    #ifndef NDEBUG
    assert(variables && asm_file && byte_array && count_elems);
    #endif

    byte_array[*count_elems] = code_command;
    *count_elems += 1;
    bool isError = true;
    while ((variables->ch = fgetc(asm_file)) == ' ')
    ;
    if (variables->ch != ':' || (fscanf(asm_file, "%d", &variables->argument) != 1))
    {
        isError = true;
        fprintf(stdout, "ERROR: invalid name of label in %zuth element\n", *count_elems);
        return ERR_INV_LABEL_asm;
    }

    for (int index = 0; index < NUMBER_LABELS; index++)
    {
        if (labels[index].name == variables->argument)
        {
            //printf("variables->argument = %d\n", variables->argument);
            //printf("label[%d] = %d\n", index, labels[index].name);
            //printf("label_adress = %d\n", labels[index].adress_label - 1);
            isError = false;
            labels[index].isUsed = true;
            byte_array[*count_elems] = labels[index].adress_label - 1;
            break;
        }
        if (labels[index].name == BAD_VALUE)
        {
            labels[index].name = variables->argument;
            isError = false;
            labels[index].isUsed = true;
            labels[index].adress_label = (label_t)*count_elems;
            break;
        }
    }
    *count_elems += 1;

    #ifndef NDEBUG
    assert(variables && asm_file && byte_array && count_elems);
    #endif

    //printf("is error in scanJump = %d\n", variables->flag_error);

    return (isError) ? ERR_INV_LABEL_asm : HAVE_NO_ERRORS_asm;
}

void setError (size_t count_line, asm_error_t code_error)
{
    asm_errors[code_error].status = true;
    asm_errors[code_error].code_asm_error = code_error;
    for (int index = 0; index < MAX_COUNT_ERR; index++)
    {
        if (asm_errors[code_error].error_lines[index].line == BAD_VALUE)
        {
            asm_errors[code_error].error_lines[index].line = (int)count_line;
            break;
        }
    }

    return;
}

void setRegistr (int* byte_array, help_var_t* variables, size_t* count_elems, FILE* asm_file, code_t command)
{
    #ifndef NDEBUG
    assert(byte_array && variables && count_elems && asm_file);
    #endif

    fscanf(asm_file, "%s", variables->str_arg);
    //printf("str_arg in PUSHR = %s\n", variables.str_arg);
    getStrArg(variables);
    byte_array[*count_elems] = command;
    *count_elems += 1;
    //printf("code_str_arg in PUSHR = %d\n", variables.code_str_arg);
    byte_array[*count_elems] = variables->code_str_arg;
    *count_elems += 1;

    #ifndef NDEBUG
    assert(byte_array && variables && count_elems && asm_file);
    #endif

    return;
}

void setCode(help_var_t* variables, int* byte_array, size_t* count_elems)
{
    #ifndef NDEBUG
    assert(variables && byte_array && count_elems);
    #endif

    switch (variables->code_command)
    {
        case code_POP:
            SET_CODE(POP)
        case code_OUT:
            SET_CODE(OUT)
        case code_ADD:
            SET_CODE(ADD)
        case code_SUB:
            SET_CODE(SUB)
        case code_MUL:
            SET_CODE(MUL)
        case code_DIV:
            SET_CODE(DIV)
        case code_SQRT:
            SET_CODE(SQRT)
        case code_RET:
            SET_CODE(RET)
        case code_DUMP:
            SET_CODE(DUMP)
        case code_HLT:
            SET_CODE(HLT)
        default:
            #ifndef NDEBUG
            assert(0 && "invalid code_command\n");
            #endif
    }

    #ifndef NDEBUG
    assert(variables && byte_array && count_elems);
    #endif

    return;
}

void setCodeWithArg (help_var_t* variables, int* byte_array, FILE* asm_file, size_t* count_elems, size_t count_lines)
{
    #ifndef NDEBUG
    assert(variables && asm_file && count_elems);
    #endif

    switch(variables->code_command)
    {
        case code_PUSH:
            fscanf(asm_file, "%d", &variables->argument);
            byte_array[*count_elems] = code_PUSH;
            *count_elems += 1;
            byte_array[*count_elems] = variables->argument;
            *count_elems += 1;
            break;
        case code_PUSHR:
            setRegistr(byte_array, variables, count_elems, asm_file, code_PUSHR);
            break;
        case code_POPR:
            setRegistr(byte_array, variables, count_elems, asm_file, code_POPR);
            break;
        case code_PUSHM:
            setCodeRAM(byte_array, variables, asm_file, count_elems, code_PUSHM);
            break;
        case code_POPM:
            setCodeRAM(byte_array, variables, asm_file, count_elems, code_POPM);
            break;
        case code_JMP:
            SET_JUMP(JMP)
        case code_JB:
            SET_JUMP(JB)
        case code_JBE:
            SET_JUMP(JBE)
        case code_JA:
            SET_JUMP(JA)
        case code_JAE:
            SET_JUMP(JAE)
        case code_JE:
            SET_JUMP(JE)
        case code_JNE:
            SET_JUMP(JNE)
        case code_CALL:
            //printf("case code_call active!\n");
            SET_JUMP(CALL)
        default:
            fprintf(stderr, "invalid command, try again\n");
            break;
    }

    #ifndef NDEBUG
    assert(variables && asm_file && count_elems);
    #endif

    return;
}

void setCodeRAM (int* byte_array, help_var_t* variables, FILE* asm_file, size_t* count_elems, code_t code_RAM_command)
{
    #ifndef NDEBUG
    assert(variables && byte_array && asm_file && count_elems);
    #endif

    skipSpaces(asm_file);
    variables->ch = fgetc(asm_file);
    //printf("first elem of argument in %s = %c\n", __func__, variables->ch);
    fscanf(asm_file, "%3s", variables->str_arg);
    getStrArg(variables);
    variables->ch = fgetc(asm_file);
    byte_array[*count_elems] = code_RAM_command;
    *count_elems += 1;
    byte_array[*count_elems] = variables->code_str_arg;
    *count_elems += 1;

    #ifndef NDEBUG
    assert(variables && asm_file && count_elems);
    #endif

    return;
}
