#include "assembler.h"
#include "calculator.h"
#include "Files.h"
#include "generalObjectForStack.h"
#include "assembler.h"
#include "../Processor/processor_PASH.h"
#include "stackFuncs.h"
#include "Utils_asm.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#define NDEBUG
#define N_DEBUD 1L
#define FIRST_SIZE_B_T 100
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
    #if N_DEBUG
    assert(byte_code && size_byte_code);
    #endif

    byte_code = (int*)calloc(FIRST_SIZE_B_T, sizeof(int));

    #if N_DEBUG
    assert(byte_code && size_byte_code);
    #endif

    return HAVE_NO_ERRORS_asm;
}
bool writeByteCode (int* byte_array, FILE* asm_file,
                           size_t* count_elems)
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
        if (variables.ind_error + 1 == MAX_COUNT_ERR) return true;
        skipSpaces(asm_file);
        if (checkLabel(&variables, asm_file, count_elems, byte_array, count_lines, &count_labels))
        {
            continue;
        }
        count_lines = *count_elems + count_labels - count_args;
        if (variables.ch == EOF) // check EOF
        {
            break;
        }
        //printf(" CH = %c\n", variables.ch);
        //printf(" count_elems = %zu\n\n", *count_elems);
        ungetc(variables.ch, asm_file); // возвращаю первый символ обратно в файл
        fscanf(asm_file, "%5s", &variables.command);
        printf("variables.command after fscanf = %s\n", variables.command);
        //printf("command = %s\n", variables.command);
        switch (getCodeCommand(&variables))
        {
            case HAVE_ERROR:
                //printf("have ERROR!\n");
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

    //printf("COUNT ARGUMENTS IS %zu\n", count_args);
    byte_array[0] = (int)*count_elems;
    //printf("in the end of writeByteCode variables.flag_error is %d\n", variables.flag_error);

    #ifndef NDEBUG
    assert(byte_array != nullptr && asm_file != nullptr && count_elems != nullptr);
    #endif

    return variables.flag_error;
}

isArg getCodeCommand (help_var_t* variables)
{
    //printf("name_command [%p]\n", name_command);
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

    return HAVE_ERROR;
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
    else if (strcmp(variables->arguments.PUSHM, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_PUSHM_pr;
    }
    else if (strcmp(variables->arguments.POPM, variables->str_arg) == 0)
    {
        variables->code_str_arg = code_POPM_pr;
    }

    return;
}

// void getAdressRAM (help_var_t* variables)
// {
//     if (variables->str_arg[0] != '[')
//     {
//
//     }
// }

asm_error_t scanLabel (FILE* asm_file, size_t* count_elems, int* byte_code)
{
    #if N_DEBUG
    assert(asm_file && count_elems && byte_code);
    #endif

    //printf("count elems before scanLabel = %zu\n", *count_elems);
    label_t curr_label = 0;
    bool isReadLabel = false;
    int temp_count_elems = 0;
    if (fscanf(asm_file, "%d", &curr_label) != 1)
    {
        printf("error\n");
        return ERR_INV_LABEL_asm;
    }
    int ch = fgetc(asm_file);
    if (isspace(ch) == false)
    {
        return ERR_INV_LABEL_asm;
    }
    ungetc(ch, asm_file);
    for (int index = 0; index < NUMBER_LABELS; index++)
    {
        if (labels[index].isUsed && labels[index].isInit == false && labels[index].name == curr_label)
        {
            isReadLabel = true;
            labels[index].isInit = true;
            temp_count_elems = labels[index].adress_label;
            labels[index].adress_label = (int)*count_elems;
            byte_code[temp_count_elems] = (int)(*count_elems - 1);
            //printf("pizdec eto %d - label_name\n", curr_label);
            *count_elems += 1;
            break;
        }
        if (labels[index].isInit == false && labels[index].name == BAD_VALUE)
        {
            isReadLabel = true;
            labels[index].name = curr_label;
            labels[index].adress_label = (label_t)*count_elems;
            //printf("label_adress[%d] in scanLabel = %d\n", index, labels[index].adress_label);
            *count_elems += 1;
            labels[index].isInit = true;
            break;
        }
    }
    //printf("count elems after scanLabel = %zu\n", *count_elems);

    #if N_DEBUG
    assert(asm_file && count_elems && byte_code);
    #endif

    return (isReadLabel) ? HAVE_NO_ERRORS_asm : ERR_MAX_LABELS_asm;
}

asm_error_t scanJump (help_var_t* variables, FILE* asm_file, code_t code_command, int* byte_array, size_t* count_elems)
{
    #if N_DEBUG
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

    #if N_DEBUG
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
    //printf("asm_errors[code_error].error_lines[variables->ind_error].line IS %d\n", asm_errors[code_error].error_lines[variables->ind_error].line);

    return;
}

void labelsVerify (int* byte_code, size_t count_elems)
{
    #if N_DEBUG
    assert(byte_code && count_elems != 0);
    #endif

    for (size_t index = 0; index < count_elems; index++)
    {
        if (byte_code[index] == code_JMP)
        {
            for (int ind = 0; ind < NUMBER_LABELS; ind++)
            {
                if (labels[ind].name == byte_code[index])
                {
                    if (labels[ind].isUsed == true && labels[ind].isInit == false)
                    {
                        fprintf(stderr, "invalid label on %zuth element\n", index);
                    }
                }
            }
        }
    }
    #if N_DEBUG
    assert(byte_code && count_elems != 0);
    #endif

    return;
}

void skipSpaces(FILE* input_file)
{
    #if N_DEBUG
    assert(input_file);
    #endif

    int ch = fgetc(input_file);
    while (isspace(ch))
        ch = fgetc(input_file);
    ungetc(ch, input_file);

    return;
}

void skipString (FILE* asm_file)
{
    #if N_DEBUG
    assert(asm_file);
    #endif

    int ch = 0;
    while((ch = fgetc(asm_file)) != '\n')
    ;

    return;
}

bool scanComment (FILE* asm_file)
{
    skipSpaces(asm_file);
    fseek(asm_file, -1L, SEEK_CUR);
    int ch = fgetc(asm_file);
    if (ch == '\n')
    {
        return false;
    }
    ch = fgetc(asm_file);
    if (ch == ';')
    {
        skipString(asm_file);
        return false;
    }
    skipString(asm_file);


    return true;
}

void setRegistr (int* byte_array, help_var_t* variables, size_t* count_elems, FILE* asm_file, code_t command)
{
    #if N_DEBUG
    assert(byte_array && variables && count_elems && asm_file);
    #endif

    fscanf(asm_file, "%s\n", variables->str_arg);
    //printf("str_arg in PUSHR = %s\n", variables.str_arg);
    getStrArg(variables);
    byte_array[*count_elems] = command;
    *count_elems += 1;
    //printf("code_str_arg in PUSHR = %d\n", variables.code_str_arg);
    byte_array[*count_elems] = variables->code_str_arg;
    *count_elems += 1;

    #if N_DEBUG
    assert(byte_array && variables && count_elems && asm_file);
    #endif

    return;
}

void setElemRAM (int* byte_array, help_var_t* variables, size_t* count_elems, FILE* asm_file, code_t command)
{
    #if N_DEBUG
    assert(byte_array && variables && count_elems && asm_file);
    #endif

    // ...

    #if N_DEBUG
    assert(byte_array && variables && count_elems && asm_file);
    #endif

    return;
}

bool checkLabel (help_var_t* variables, FILE* asm_file, size_t* count_elems, int* byte_array, size_t count_lines, size_t* count_labels)
{
    #ifndef NDEBUG
    assert(variables && asm_file && byte_array && count_labels);
    #endif

    variables->ch = fgetc(asm_file);
        if (variables->ch == ':')
        {
            *count_labels += 1;
            switch (scanLabel(asm_file, count_elems, byte_array))
            {
                case HAVE_NO_ERRORS_asm:
                    break;
                case ERR_INV_LABEL_asm:
                    variables->flag_error = true;
                    setError(count_lines, ERR_UNEXP_SYM_asm);
                    variables->ind_error++;
                case ERR_MAX_LABELS_asm:
                    variables->flag_error = true;
                    setError(count_lines, ERR_UNEXP_SYM_asm);
                    variables->ind_error++;
                default:
                    fprintf(stderr, "assert called from %s:%d - invalid return value from %s\n", __FILE__, __LINE__, __func__);
                    assert(0);
            }
            if (scanComment(asm_file))
            {
            variables->flag_error = true;
            setError(count_lines, ERR_UNEXP_SYM_asm);
            variables->ind_error++;
            }
            skipString(asm_file);
            return true;
        }

    #ifndef NDEBUG
    assert(variables && asm_file && byte_array && count_labels);
    #endif

    return false;
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
            fscanf(asm_file, "%d\n", &variables->argument);
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

void checkComment (help_var_t* variables, FILE* asm_file, size_t count_lines)
{
    #ifndef NDEBUG
    assert(variables && asm_file);
    #endif

    if (scanComment(asm_file))
    {
        //printf("scanComment was truej!!!!!\n");
        variables->flag_error = true;
        setError(count_lines, ERR_UNEXP_SYM_asm);
        variables->ind_error++;

    }

    #ifndef NDEBUG
    assert(variables && asm_file);
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
    printf("first elem of argument in %s = %c\n", __func__, variables->ch);
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
