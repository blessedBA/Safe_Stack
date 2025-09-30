#include "calculator.h"
#include "operationsCalc.h"
#include "safetyOfStack.h"
#include "stackFuncs.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int_error_t calculatorOn (my_stack_t* stack)
{
    int_error_t  code_error = CHECK_STACK;

    commandStorage command = {}; // стандартные значения указаны в определении

    FILE* input_file = stdin;
    FILE* output_file = stdout;
    char name_command[5] = "";
    code_t code_command = code_NAN;
    stack_elem_t number = BAD_VALUE;
    stack_elem_t value = BAD_VALUE;
    while(strcmp(name_command, command.HLT))
    {
        if (getCommand(&command, &code_command, &number, name_command, input_file) == false)
        {
            fprintf(stderr, "invalid command, try again!!!\n");
            continue;
        }
        switch (code_command)
        {
            case code_NAN:
                fprintf(stderr, "invalid command, try again!!!\n");
                continue;
                break;
            case code_PUSH:
                stackPush(stack, number);
                break;
            case code_POP:
                stackPop(stack, &value);
                break;
            case code_OUT:
                printStack(stack, output_file);
                break;
            case code_ADD:
                stackAdd(stack);
                break;
            case code_SUB:
                stackSub(stack);
                break;
            case code_MUL:
                stackMul(stack);
                break;
            case code_DIV:
                stackDiv(stack);
                break;
            case code_SQRT:
                stackSqrt(stack);
                break;
            case code_HLT:
                break;
            default:
                fprintf(stderr, "invalid command, try again!!!\n");
        }
    }
    fprintf(output_file, "GOOD LUCK MEOW\n");
    fclose(input_file);
    fclose(output_file);

    CHECK_STACK

    return HAVE_NO_ERRORS;
}

bool getCommand (commandStorage* command, code_t* code_command, stack_elem_t* number,
                                                 char* name_command, FILE* input_file)
{
    assert(command != nullptr && code_command != nullptr && number != nullptr && input_file != nullptr);
    *code_command = code_NAN;
    if (fscanf(input_file, "%4s", name_command) != 1)
    {
        name_command[0] = '\0';
        return false;
    }
    if (strcmp(command->PUSH, name_command) == 0)
    {
        *code_command = code_PUSH;
        if (fscanf(input_file, "%d", number) != 1)
        {
            return false;
        }
    }
    else if (strcmp(command->POP, name_command) == 0)
    {
        *code_command = code_POP;
    }
    else if (strcmp(command->OUT, name_command) == 0)
    {
        *code_command = code_OUT;
    }
    else if (strcmp(command->ADD, name_command) == 0)
    {
        *code_command = code_ADD;
    }
    else if (strcmp(command->SUB, name_command) == 0)
    {
        *code_command = code_SUB;
    }
    else if (strcmp(command->MUL, name_command) == 0)
    {
        *code_command = code_MUL;
    }
    else if (strcmp(command->DIV, name_command) == 0)
    {
        *code_command = code_DIV;
    }
    else if (strcmp(command->SQRT, name_command) == 0)
    {
        *code_command = code_SQRT;
    }
    else if (strcmp(command->HLT, name_command) == 0)
    {
        *code_command = code_HLT;
    }
    else
    {
        return false;
    }
    return true;
}

