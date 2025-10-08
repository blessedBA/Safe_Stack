#include "calculator.h"
#include "errors.h"
#include "operationsCalc.h"
#include "processor_PASH.h"

#include <assert.h>
#include <stdio.h>

#define FIRST_ELEMENT      1
#define ERROR_COUNT_ELEMS  0
#define NUMBER_ERRORS      6

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
    assert(processor && byte_file);
    #endif
    if (!fread(processor->byte_code.b_code, processor->byte_code.count_elems, sizeof(byte_code_t), byte_file))
    {
        return ERR_READ_BT;
    }
    return NO_ERRORS_PR;
}

int_error_t doByteCode (Processor_t* processor)
{

    int_error_t code_error = 0;
    stack_elem_t deleted_elem = 0;
    for (size_t index = 1; index < processor->byte_code.count_elems; index++)
    {
        switch (processor->byte_code.b_code[index])
        {
            case code_PUSH:
                processor->P_C = code_PUSH;
                stackPush(&processor->stack1, processor->byte_code.b_code[++index]);
                break;
            case code_POP:
                processor->P_C = code_POP;
                stackPop(&processor->stack1, &deleted_elem);
                break;
            case code_OUT:
                processor->P_C = code_OUT;
                printStack(&processor->stack1, stdin);
                break;
            case code_ADD:
                processor->P_C = code_ADD;
                stackAdd(&processor->stack1);
                break;
            case code_SUB:
                processor->P_C = code_SUB;
                stackSub(&processor->stack1);
                break;
            case code_MUL:
                processor->P_C = code_MUL;
                stackMul(&processor->stack1);
                break;
            case code_DIV:
                processor->P_C = code_DIV;
                stackDiv(&processor->stack1);
                break;
            case code_SQRT:
                processor->P_C = code_SQRT;
                stackSqrt(&processor->stack1);
                break;
            case code_PUSHR:
                processor->P_C = code_PUSHR;
                writeToRegistr(processor, processor->byte_code.b_code[++index]);
                break;
            case code_POPR:
                processor->P_C = code_POPR;
                getValue(processor, processor->byte_code.b_code[++index]);
                break;
            case code_HLT:
                processor->P_C = code_HLT;
                return code_error;
            default:
                code_error |= code_ERR_INV_CODE;
        }
    }

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
        //procDump (processor, global_code_pr_error, &f_data_pr);
        return global_code_pr_error;
    }

    return global_code_pr_error;
}

void writeToRegistr (Processor_t* processor, int code_registr)
{
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

    return;
}

void getValue (Processor_t* processor, int code_registr)
{
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

    return;
}

void procDump (Processor_t* processor, int global_code_error, func_data* f_data_pr)
{
    #if N_DEBUG
    assert(f_data_pr);
    #endif
    FILE* output_file = stderr;
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
        fprintf(output_file, "byte_code [%p]\n", processor->byte_code);
    }
    // ...

    return;
}
