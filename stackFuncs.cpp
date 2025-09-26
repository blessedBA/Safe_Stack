#include "stackFuncs.h"
#include "Files.h"
#include "safetyOfStack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK_STACK code_error = stackVerify(stack, __FILE__, __func__, __LINE__); \
                    if (code_error) \
                    { \
                        return code_error; \
                    }

void decimalToBinary(int decimalNum, FILE* output_file) {
    if (decimalNum == 0) {
        return;
    }
    int binaryNum[32];
    int i = 0;
    while (decimalNum > 0) {
        binaryNum[i] = decimalNum % 2;
        decimalNum = decimalNum / 2;
        i++;
    }
    for (int j = i - 1; j >= 0; j--) {
        fprintf(output_file, "%d", binaryNum[j]);
    }
    fprintf(output_file, "\n");
}

int_error_t stackPush (my_stack_t* stack, stack_elem_t number)
{
    int_error_t  code_error = HAVE_NO_ERRORS;
    CHECK_STACK

    stack->data[stack->size++] = number;

    CHECK_STACK

    return HAVE_NO_ERRORS;
}

int_error_t stackInit (my_stack_t* stack)
{
    int_error_t  code_error = HAVE_NO_ERRORS;

    stack->capasity = 10;
    stack->size = 0;
    stack->data = (stack_elem_t*)calloc((size_t)stack->capasity, sizeof(stack_elem_t));
    for (stack_capasity_t index = 0; index < stack->capasity; index++)
    {
        stack->data[index] = BAD_VALUE;
    }
    CHECK_STACK

    return HAVE_NO_ERRORS;
}

int_error_t stackPop (my_stack_t* stack, stack_elem_t* value)
{
    int_error_t  code_error = HAVE_NO_ERRORS;
    CHECK_STACK

    *value = stack->data[stack->size - 1];
    stack->data[--stack->size] = BAD_VALUE;
    CHECK_STACK

    return HAVE_NO_ERRORS;
}

int_error_t printStack (my_stack_t* stack)
{
    int_error_t  code_error = HAVE_NO_ERRORS;
    CHECK_STACK

    // printf("stack:\n");
    // printf("size = %lld\n", stack->size);
    for (stack_size_t index = 0; index < stack->size; index++)
    {
        printf("[%lld] = %d\n", index, stack->data[index]);
    }

    CHECK_STACK

    return HAVE_NO_ERRORS;
}

int_error_t stackDestroy (my_stack_t* stack)
{
    int_error_t  code_error = HAVE_NO_ERRORS;
    CHECK_STACK

    free(stack->data);
    stack->data = nullptr;
    stack->size = BAD_VALUE;
    stack->capasity = BAD_VALUE;

    return HAVE_NO_ERRORS;
}

void stackDump (my_stack_t* stack, int GLOBAL_ERROR, func_data* f_data)
{
    printf("stackDump was called!\n");
    FILE* output_file = fopen(LOG_FILE, "a");
    assert(output_file != nullptr && "failed to open file!!!\n");

    fprintf(output_file, "stackDump called from %s: function %s: %d\n", f_data->file_name,
                                                                        f_data->func_name,
                                                                        f_data->line);
    fprintf(output_file, "error code: ");
    decimalToBinary(GLOBAL_ERROR, output_file);
    if (stack != nullptr)
    {
        fprintf(output_file, "stack1 [%p]\n",    stack);
    }
    else
    {
        fprintf(output_file, "stack1 !!!![nullptr]!!!!\n");
    }
    fprintf(output_file, "{ \n");
    fprintf(output_file, "\t size     = %lld\n", stack->size);
    fprintf(output_file, "\t capasity = %zd\n", stack->capasity);
    if (stack->data != nullptr)
    {
        fprintf(output_file, "\t data [%p]\n", stack->data);
    }
    else
    {
        fprintf(output_file, "\t data !!!![nullptr]!!!!\n");
    }
    fprintf(output_file, "\n");
    if (stack->data != nullptr)
    {
        if (stack->size > 0 && stack->size <= stack->capasity)
        {
            for (stack_size_t index = 0; index < stack->size; index++)
            {
                fprintf(output_file, "\t*[%lld] = %d\n", index, stack->data[index]);
            }
            for (stack_capasity_t index_poizon = (stack_capasity_t)stack->size;
                index_poizon < stack->capasity; index_poizon++)
            {
                fprintf(output_file, "\t [%zd] = %d (TRASH)\n", index_poizon,
                                                                 stack->data[index_poizon]);
            }
        }
        else if (stack->capasity > 0)
        {
            for (stack_capasity_t index_poizon = 0;
                index_poizon < stack->capasity; index_poizon++)
            {
                fprintf(output_file, "\t [%zd] = %d (TRASH)\n", index_poizon,
                                                                 stack->data[index_poizon]);
            }
        }
    }
    else
    {
        fprintf(output_file, "chini svou programmy\n");
    }
    fprintf(output_file, "} \n");
    fclose(output_file);

    return;
}

int_error_t addElements (my_stack_t* stack)
{
    int_error_t  code_error = HAVE_NO_ERRORS;
    CHECK_STACK
    for (stack_elem_t index = 0; index < stack->size; index++)
    {
        stack->data[index] = index * 10;
    }

    CHECK_STACK

    return HAVE_NO_ERRORS;
}
