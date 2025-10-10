#include "calculator.h"
#include "errors.h"
#include "operationsCalc.h"
#include "processor_PASH.h"
#include "stackJumps.h"

#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#define FIRST_ELEMENT      1
#define ERROR_COUNT_ELEMS  0
#define NUMBER_ERRORS      6
#ifndef N_DEBUG
#define N_DEBUG            1
#endif
#define PRINT_BEGINNING_OF_DUMP fprintf(output_file, "//--------------------------------processorDUMP-----------------------------//");

storageProcErrors errors_pt[NUMBER_ERRORS] = { {NO_ERRORS_PR,   code_NO_ERRORS_PR,   "your processor is great!!!", true },
                                               {ERR_FAIL_OP_BT, code_ERR_FAIL_OP_BT, "failed to open byte file",   false},
                                               {ERR_READ_BT,    code_ERR_READ_BT,    "failed to read byte file",   false},
                                               {ERR_INV_PROC,   code_ERR_INV_PROC,   "INVALID PROCESSOR       ",   false},
                                               {ERR_INV_STACK,  code_ERR_INV_STACK,  "invalid STACK           ",   false},
                                               {ERR_INV_B_CODE, code_ERR_INV_B_CODE, "invalid byte code       ",   false} };


int_error_t global_code_pr_error = 0;
size_t checkCountElems(FILE* byte_file)
{
    #if N_DEBUG
    assert(byte_file);
    #endif
    size_t count_elems = 0;
    fread(&count_elems, FIRST_ELEMENT, sizeof(int), byte_file);

    return (count_elems > 1) ? count_elems : ERROR_COUNT_ELEMS;
}


int_error_t readByteFile(Processor_t* processor, FILE* byte_file)
{
    #if N_DEBUG
    assert(byte_file);
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    if (!fread(processor->byte_code.b_code, processor->byte_code.count_elems, sizeof(byte_code_t), byte_file))
    {
        return ERR_READ_BT;
    }

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
    stack_elem_t deleted_elem = 0;
    stack_elem_t last_num = BAD_VALUE;
    stack_elem_t prelast_num = BAD_VALUE;
    for ( ; processor->P_C < processor->byte_code.count_elems; processor->P_C++)
    {
        switch (processor->byte_code.b_code[processor->P_C])
        {
            case code_PUSH:
                processor->curr_command = code_PUSH;
                stackPush(&processor->stack1, processor->byte_code.b_code[++processor->P_C]);
                break;
            case code_POP:
                processor->curr_command = code_POP;
                stackPop(&processor->stack1, &deleted_elem);
                break;
            case code_OUT:
                processor->curr_command = code_OUT;
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
                writeToRegistr(processor, processor->byte_code.b_code[++processor->P_C]);
                break;
            case code_POPR:
                processor->curr_command = code_POPR;
                getValue(processor, processor->byte_code.b_code[++processor->P_C]);
                break;
            case code_JMP:
                processor->curr_command = code_JMP;
                printf("CODE_JMP\n");
                usleep(3e6);
                stackJMP(processor, processor->byte_code.b_code[++processor->P_C]);
                break;
            case code_JB:
                processor->curr_command = code_JB;
                printf("CODE_JB\n");
                usleep(3e6);
                stackJB(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
                break;
            case code_JBE:
                processor->curr_command = code_JBE;
                printf("CODE_JBE\n");
                usleep(3e6);
                stackJBE(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
                break;
            case code_JA:
                processor->curr_command = code_JA;
                printf("CODE_JA\n");
                usleep(3e6);
                stackJA(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
                break;
            case code_JAE:
                processor->curr_command = code_JAE;
                printf("CODE_JAE\n");
                usleep(3e6);
                stackJAE(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
                break;
            case code_JE:
                processor->curr_command = code_JE;
                printf("CODE_JE\n");
                usleep(3e6);
                stackJE(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
                break;
            case code_JNE:
                processor->curr_command = code_JNE;
                printf("CODE_JNE\n");
                usleep(3e6);
                stackJNE(processor, processor->byte_code.b_code[++processor->P_C], &prelast_num, &last_num);
                break;
            case code_HLT:
                processor->curr_command = code_HLT;
                processor->P_C++;
                return code_error;
            default:
                code_error |= code_ERR_INV_CODE;
        }
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
    #if N_DEBUG
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
    fprintf(output_file, "processor [%p]\n", processor);
    if (processor == nullptr)
    {
        return;
    }
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
