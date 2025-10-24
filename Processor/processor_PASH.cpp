#include "calculator.h"
#include "errors.h"
#include "operationsCalc.h"
#include "processor_PASH.h"
#include "RAM.h"
#include "stackJumps.h"

#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE_RAM 110
#define FIRST_ELEMENT      1
#define ERROR_COUNT_ELEMS  0
#define NUMBER_ERRORS      6
#ifndef N_DEBUG
#define N_DEBUG            1L
#endif
#define PRINT_BEGINNING_OF_DUMP fprintf(output_file, "//--------------------------------processorDUMP-----------------------------//");

storageProcErrors errors_pt[NUMBER_ERRORS] = { {NO_ERRORS_PR,   code_NO_ERRORS_PR,   "your processor is great!!!", true },
                                               {ERR_FAIL_OP_BT, code_ERR_FAIL_OP_BT, "failed to open byte file",   false},
                                               {ERR_READ_BT,    code_ERR_READ_BT,    "failed to read byte file",   false},
                                               {ERR_INV_PROC,   code_ERR_INV_PROC,   "INVALID PROCESSOR       ",   false},
                                               {ERR_INV_STACK,  code_ERR_INV_STACK,  "invalid STACK           ",   false},
                                               {ERR_INV_B_CODE, code_ERR_INV_B_CODE, "invalid byte code       ",   false} };

void dumpRAM (Processor_t* processor);

Processor_t* creatProcessor()
{
    Processor_t* processor = (Processor_t*)calloc(1, sizeof(Processor_t));
    if (processor == nullptr) return nullptr;

    processor->sizeRAM = SIZE_RAM;
    processor->RAM = createRAM(processor->sizeRAM);
    processor->byte_code.b_code   = nullptr;
    processor->byte_code.count_elems = 0;
    processor->P_C = 0;
    processor->curr_command = 0;
    processor->curr_error   = NO_ERRORS_PR;
    stackInit(&processor->stack1);
    stackInit(&processor->stackCall);

    #ifndef NDEBUG
    assert(&processor);
    #endif

    return processor;
}

int_error_t global_code_pr_error = 0;
size_t checkCountElems(FILE* byte_file)
{
    #if N_DEBUG
    assert(byte_file);
    #endif
    size_t count_elems = 0;
    fread(&count_elems, FIRST_ELEMENT, sizeof(int), byte_file);
    printf("count_elems = %zu\n", count_elems);
    return (count_elems > 1) ? count_elems : ERROR_COUNT_ELEMS;
}


int_error_t readByteFile(Processor_t* processor, FILE* byte_file)
{
    #if N_DEBUG
    assert(byte_file);
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif
    printf("IN %s: processor->byte_code.count_elems = %zu\n", __func__, processor->byte_code.count_elems);
    printf("IN %s: processor [%p]\n", __func__, processor);
    printf("IN %s: processor->byte_code.b_code [%p]\n", __func__, processor->byte_code.b_code);
    printf("IN %s: byte_file [%p]\n", __func__, byte_file);
    if (!fread(processor->byte_code.b_code, processor->byte_code.count_elems, sizeof(byte_code_t), byte_file))
    {
        printf("FAILED FREAD\n");
        return ERR_READ_BT;
    }
    printf("first elem in byte_code is %d\n", processor->byte_code.b_code[0]);
    #if N_DEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    return NO_ERRORS_PR;
}

int_error_t doByteCode (Processor_t* processor)
{
    #if N_DEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    int_error_t code_error = 0;
    stack_elem_t last_num = BAD_VALUE;
    stack_elem_t prelast_num = BAD_VALUE;
    bool isExit = false;
    //printf("count elems = %d\n", processor->byte_code.count_elems);
    //printf("P_C = %d\n", processor->P_C);
    for ( ; processor->P_C < processor->byte_code.count_elems; processor->P_C++)
    {
        if (doCommand(processor, &code_error, last_num, prelast_num, &isExit)) return code_error;
        if (isExit) return code_error;
    }

    #if N_DEBUG
    code_error |= procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    return code_error;
}


int_error_t procVerify (Processor_t* processor, const char* file_name,
                                                const char* func_name,
                                                int         line)
{
    #ifndef NDEBUG
    assert(file_name && func_name);
    #endif

    func_data f_data_pr = {.file_name = file_name, .func_name = func_name, .line = line};
    if (processor == nullptr)
    {
        processor->curr_error = ERR_INV_PROC;
        errors_pt[NO_ERRORS_PR].isError = false;
        errors_pt[ERR_INV_PROC].isError = true;
        global_code_pr_error |= ERR_INV_PROC;
        return global_code_pr_error;
    }
    if (&processor->stack1 == nullptr)
    {
        processor->curr_error = ERR_INV_STACK;
        errors_pt[NO_ERRORS_PR].isError = false;
        errors_pt[ERR_INV_STACK].isError = true;
        global_code_pr_error |= code_ERR_INV_STACK;
    }
    if (&processor->byte_code == nullptr)
    {
        processor->curr_error = ERR_INV_B_CODE;
        errors_pt[NO_ERRORS_PR].isError = false;
        errors_pt[ERR_INV_B_CODE].isError = true;
        global_code_pr_error |= code_ERR_INV_B_CODE;
    }
    if (errors_pt[NO_ERRORS_PR].isError == false)
    {
        procDump(processor, global_code_pr_error, &f_data_pr);
        return global_code_pr_error;
    }

    return global_code_pr_error;
}

void writeToRegistr (Processor_t* processor, int code_registr)
{
    #ifndef NDEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    code_registr = (str_arg_t)code_registr;
    stack_elem_t value = 0;
    switch (code_registr)
    {
        case code_RAX:
            stackPop(&processor->stack1, &value);
            processor->registers.RAX = value;
            break;
        case code_RBX:
            stackPop(&processor->stack1, &value);
            processor->registers.RBX = value;
            break;
        case code_RCX:
            stackPop(&processor->stack1, &value);
            processor->registers.RCX = value;
            break;
        case code_RDX:
            stackPop(&processor->stack1, &value);
            processor->registers.RDX = value;
            break;
        case code_REX:
            stackPop(&processor->stack1, &value);
            processor->registers.REX = value;
            break;
        case code_RFX:
            stackPop(&processor->stack1, &value);
            processor->registers.RFX = value;
            break;
        case code_RGX:
            stackPop(&processor->stack1, &value);
            processor->registers.RGX = value;
            break;
        case code_RHX:
            stackPop(&processor->stack1, &value);
            processor->registers.RHX = value;
            break;
        default:
            assert(0 && "invalid registr\n");
    }

    #if N_DEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    return;
}

void getValue (Processor_t* processor, int code_registr)
{
    #if N_DEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    switch (code_registr)
    {
        case code_RAX:
            stackPush(&processor->stack1, (int)processor->registers.RAX);
            break;
        case code_RBX:
            stackPush(&processor->stack1, (int)processor->registers.RBX);
            break;
        case code_RCX:
            stackPush(&processor->stack1, (int)processor->registers.RCX);
            break;
        case code_RDX:
            stackPush(&processor->stack1, (int)processor->registers.RDX);
            break;
        case code_REX:
            stackPush(&processor->stack1, (int)processor->registers.REX);
            break;
        case code_RFX:
            stackPush(&processor->stack1, (int)processor->registers.RFX);
            break;
        case code_RGX:
            stackPush(&processor->stack1, (int)processor->registers.RGX);
            break;
        case code_RHX:
            stackPush(&processor->stack1, (int)processor->registers.RHX);
            break;
        default:
            assert(0 && "invalid registr\n");
    }

    #if N_DEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    return;
}

void procDump (Processor_t* processor, int global_code_error, func_data* f_data_pr)
{
    #if N_DEBUG
    assert(f_data_pr);
    #endif

    FILE* output_file = stderr;
    PRINT_BEGINNING_OF_DUMP
    fprintf(output_file, "processorDump was called from file %s in function %s:%d\n", f_data_pr->file_name,
                                                                                      f_data_pr->func_name,
                                                                                      f_data_pr->line);
    fprintf(output_file, "error code:\t%d\n", global_code_error);
    size_t size_st_errors = sizeof(errors_pt) / sizeof(errors_pt[0]);
    if (errors_pt[NO_ERRORS_PR].isError)
    {
        fprintf(output_file, "Errors: \n");
    }
    for (size_t index = 0; index < size_st_errors; index++)
    {
        if (errors_pt[index].isError == true)
        {
            fprintf(output_file, "%s\n", errors_pt[index].description);
        }
    }
    if (processor == nullptr)
    {
        fprintf(output_file, "FATAL ERROR: INVALID POINTER TO PROCESSOR\n");
        return;
    }
    fprintf(output_file, "processor [%p]\n", processor);
    if (&processor->byte_code != nullptr)
    {
        fprintf(output_file, "byte_code [%p]\n", &processor->byte_code);
    }
    fprintf(output_file, "processor [%p]\n", processor);
    fprintf(output_file, "programm counter: %zu\n", processor->P_C);
    fprintf(output_file, "code of current command: %zu\n", processor->curr_command);
    fprintf(output_file, "code of current error:   %d\n", processor->curr_error);

    return;
}

int_error_t doCommand (Processor_t* processor, int_error_t* code_error, stack_elem_t last_num,
                       stack_elem_t prelast_num, bool* isExit)
{
    #ifndef NDEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    assert(code_error);
    #endif

    stack_elem_t tmp = 0;
    register_t val_in_reg = 0;
    switch (processor->byte_code.b_code[processor->P_C])
    {
        case code_NAN: // for skip labels
            break;
        case code_PUSH:
            processor->curr_command = code_PUSH;
            stackPush(&processor->stack1, processor->byte_code.b_code[++processor->P_C]);
            break;
        case code_POP:
            if (processor->stack1.size == 0)
            {
                fprintf(stderr, "code_POP with size = 0!!! P_C is %zu\n", processor->P_C);
            }
            processor->curr_command = code_POP;
            stackPop(&processor->stack1, &tmp);
            break;
        case code_OUT:
            processor->curr_command = code_OUT;
            stackPop(&processor->stack1, &tmp);
            printf("OUT %d\n", tmp);
            break;
        case code_DUMP:
            processor->curr_command = code_DUMP;
            printStack(&processor->stack1, stdin);
            break;
        case code_ADD:
            processor->curr_command = code_ADD;
            stackAdd(&processor->stack1);
            break;
        case code_SUB:
            processor->curr_command = code_SUB;
            stackSub(&processor->stack1);
            break;
        case code_MUL:
            processor->curr_command = code_MUL;
            stackMul(&processor->stack1);
            break;
        case code_DIV:
            processor->curr_command = code_DIV;
            stackDiv(&processor->stack1);
            break;
        case code_SQRT:
            processor->curr_command = code_SQRT;
            stackSqrt(&processor->stack1);
            break;
        case code_PUSHR:
            processor->curr_command = code_PUSHR;
            getValue(processor, processor->byte_code.b_code[++processor->P_C]);
            break;
        case code_POPR:
            if (processor->stack1.size == 0)
            {
                fprintf(stderr, "code_POPR with size = 0!!! P_C is %zu\n", processor->P_C);
            }
            processor->curr_command = code_POPR;
            writeToRegistr(processor, processor->byte_code.b_code[++processor->P_C]);
            break;
        case code_JMP:
            processor->curr_command = code_JMP;
            printf("CODE_JMP\n");
            //usleep(3e6);
            //printf("arg with jump\n");
            stackJMP(processor, processor->byte_code.b_code[++processor->P_C]);
            break;
        case code_JB:
            processor->curr_command = code_JB;
            printf("CODE_JB\n");
            //usleep(3e6);
            stackJB(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
            break;
        case code_JBE:
            processor->curr_command = code_JBE;
            printf("CODE_JBE\n");
            //usleep(3e6);
            stackJBE(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
            break;
        case code_JA:
            processor->curr_command = code_JA;
            printf("CODE_JA\n");
            //usleep(3e6);
            stackJA(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
            break;
        case code_JAE:
            processor->curr_command = code_JAE;
            printf("CODE_JAE\n");
            //usleep(3e6);
            stackJAE(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
            break;
        case code_JE:
            processor->curr_command = code_JE;
            printf("CODE_JE\n");
            //usleep(3e6);
            stackJE(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
            break;
        case code_JNE:
            processor->curr_command = code_JNE;
            printf("CODE_JNE\n");
            usleep(3e6);
            stackJNE(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
            break;
        case code_CALL:
            processor->curr_command = code_CALL;
            printf("CODE_CALL\n");
            //usleep(3e6);
            stackPush(&processor->stackCall, processor->P_C + 1); // processor->P_C + 1 это индекс аргумента колл, еще +1 сделает в конце сам цикл
            printf("processor->P_C in CALL before jump = %d\n", processor->P_C);
            stackJMP(processor, processor->byte_code.b_code[++processor->P_C]);
            printf("processor->P_C in CALL after jump = %d\n", processor->P_C);
            break;
        case code_RET:
            processor->curr_command = code_RET;
            printf("CODE_RET\n");
            //usleep(3e6);
            stackPop(&processor->stackCall, &tmp);
            //printf("tmp = %d\n", tmp);
            stackJMP(processor, tmp);
            break;
        case code_PUSHM:
            processor->curr_command = code_PUSHM;
            getValueFromRegistr(processor, &val_in_reg);
            stackPush(&processor->stack1, processor->RAM[val_in_reg]);
            break;
        case code_POPM:
            printf("code_POPM called\n");
            if (processor->stack1.size == 0)
            {
                fprintf(stderr, "code_POPM with size = 0!!! P_C is %zu\n", processor->P_C);
            }
            processor->curr_command = code_POPM;
            stackPop(&processor->stack1, &tmp);
            printf("tmp in code_POPM after stackPop = %d\n", tmp);
            getValueFromRegistr(processor, &val_in_reg);
            printf("val_in_reg after getValueFromRegistr = %lld\n", val_in_reg);
            processor->RAM[val_in_reg] = tmp;
            printf("processor->RAM[%lld] = %d\n", val_in_reg, tmp);
            dumpRAM(processor);
            break;
        case code_HLT:
            processor->curr_command = code_HLT;
            processor->P_C++;
            *isExit = true;
            return *code_error;
        default:
            fprintf(stderr, "error in %s:%d\n", __FILE__, __LINE__);
            *code_error |= code_ERR_INV_CODE;
    }

    #ifndef NDEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    assert(code_error);
    #endif

    return *code_error;
}

void getValueFromRegistr(Processor_t* processor, register_t* var_in_reg)
{
    #ifndef NDEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    switch (processor->byte_code.b_code[++processor->P_C])
    {
        case code_RAX:
            *var_in_reg = processor->registers.RAX;
            break;
        case code_RBX:
            *var_in_reg = processor->registers.RBX;
            break;
        case code_RCX:
            *var_in_reg = processor->registers.RCX;
            break;
        case code_RDX:
            *var_in_reg = processor->registers.RDX;
            break;
        case code_REX:
            *var_in_reg = processor->registers.REX;
            break;
        case code_RFX:
            *var_in_reg = processor->registers.RFX;
            break;
        case code_RGX:
            *var_in_reg = processor->registers.RGX;
            break;
        case code_RHX:
            *var_in_reg = processor->registers.RHX;
            break;
        default:
            fprintf(stderr, "invalid code_registr in %s:%d\n", __FILE__, __LINE__);
    }

    #ifndef NDEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    return;
}
