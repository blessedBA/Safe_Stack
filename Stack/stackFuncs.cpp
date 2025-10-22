#include "stackFuncs.h"
#include "Files.h"
#include "safetyOfStack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


canary_t canaries = {.ind_l_can = 0, .ind_r_can = BAD_VALUE, .val_l_can = 4095, .val_r_can = 16383};

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
    #if N_DEBUG
    int_error_t  code_error = CHECK_STACK;
    #endif
    stack->data[1 + stack->size] = number;
    stack->size++;
    #if N_DEBUG
    code_error = CHECK_STACK;
    #endif

    return HAVE_NO_ERRORS;
}

int_error_t stackInit (my_stack_t* stack)
{
    #if N_DEBUG
    assert(stack != nullptr);
    int_error_t  code_error = HAVE_NO_ERRORS;
    #endif

    stack->capacity = 100;
    stack->size = 0;
    stack->data = (stack_elem_t*)calloc((size_t)(stack->capacity + 2), sizeof(stack_elem_t));
    #if N_DEBUG
    assert(stack->data != nullptr);
    #endif
    setCanaries(stack);
    for (stack_capasity_t index = 1; index <= stack->capacity; index++)
    {
        stack->data[index] = BAD_VALUE;
    }
    #if N_DEBUG
    code_error = CHECK_STACK
    #endif

    return HAVE_NO_ERRORS;
}

int_error_t stackPop (my_stack_t* stack, stack_elem_t* value)
{
    //printf("SIZE = %lld\n", stack->size);
    #if N_DEBUG
    int_error_t  code_error = CHECK_STACK;
    #endif
    //printf("stackpopik\n");
    *value = stack->data[stack->size]; // 
    stack->data[stack->size--] = BAD_VALUE;

    #if N_DEBUG
    code_error = CHECK_STACK
    #endif

    return HAVE_NO_ERRORS;
}

int_error_t printStack (my_stack_t* stack, FILE* output_file)
{
    #if N_DEBUG
    int_error_t  code_error = CHECK_STACK;
    #endif

    fprintf(output_file, "stack:\n");
    // printf("size = %lld\n", stack->size);
    for (stack_size_t index = 1; index <= stack->size; index++)
    {
        printf("\t[%lld] = %d\n", index, stack->data[index]);
    }
    #if N_DEBUG
    code_error = CHECK_STACK
    #endif

    return HAVE_NO_ERRORS;
}

int_error_t stackDestroy (my_stack_t* stack)
{
    #if N_DEBUG
    int_error_t  code_error = CHECK_STACK;
    #endif

    free(stack->data);
    stack->data = nullptr;
    stack->size = BAD_VALUE;
    stack->capacity = BAD_VALUE;

    return HAVE_NO_ERRORS;
}

int_error_t addElements (my_stack_t* stack)
{
    #if N_DEBUG
    int_error_t  code_error = CHECK_STACK;
    #endif

    for (stack_elem_t index = 0; index < stack->size; index++)
    {
        stack->data[index] = index * 10;
    }
    #if N_DEBUG
    code_error = CHECK_STACK
    #endif

    return HAVE_NO_ERRORS;
}
