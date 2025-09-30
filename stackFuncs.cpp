#include "stackFuncs.h"
#include "Files.h"
#include "safetyOfStack.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>



canary_t canaries = {.ind_l_can = 0, .ind_r_can = BAD_VALUE, .val_l_can = 4095, .val_r_can = 16383};
// TODO засунуть инициализацию поля canaries в main, передавать ее туда, где она используется
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
    int_error_t  code_error = CHECK_STACK;

    stack->data[1 + stack->size] = number;
    stack->size++;

    code_error = CHECK_STACK;

    return HAVE_NO_ERRORS;
}

int_error_t stackInit (my_stack_t* stack)
{
    assert(stack != nullptr);
    int_error_t  code_error = HAVE_NO_ERRORS;

    stack->capasity = 10;
    stack->size = 0;
    stack->data = (stack_elem_t*)calloc((size_t)(stack->capasity + 2), sizeof(stack_elem_t));
    assert(stack->data != nullptr);
    setCanaries(stack);
    for (stack_capasity_t index = 1; index <= stack->capasity; index++)
    {
        stack->data[index] = BAD_VALUE;
    }

    code_error = CHECK_STACK

    return HAVE_NO_ERRORS;
}

int_error_t stackPop (my_stack_t* stack, stack_elem_t* value)
{
    int_error_t  code_error = CHECK_STACK;

    *value = stack->data[stack->size];
    stack->data[stack->size--] = BAD_VALUE;

    code_error = CHECK_STACK

    return HAVE_NO_ERRORS;
}

int_error_t printStack (my_stack_t* stack, FILE* output_file)
{
    int_error_t  code_error = CHECK_STACK;

    fprintf(output_file, "stack:\n");
    // printf("size = %lld\n", stack->size);
    for (stack_size_t index = 1; index <= stack->size; index++)
    {
        printf("\t[%lld] = %d\n", index, stack->data[index]);
    }

    code_error = CHECK_STACK

    return HAVE_NO_ERRORS;
}

int_error_t stackDestroy (my_stack_t* stack)
{
    int_error_t  code_error = CHECK_STACK;

    free(stack->data);
    stack->data = nullptr;
    stack->size = BAD_VALUE;
    stack->capasity = BAD_VALUE;

    return HAVE_NO_ERRORS;
}

void stackDump (my_stack_t* stack, int GLOBAL_ERROR, func_data* f_data) // TODO сделать массив структур ошибок, обрабатывать его здесь и выводить более подробную информацию об ошибке
{
    printf("stackDump was called!\n");
    FILE* output_file = fopen(LOG_FILE, "a");
    assert(output_file != nullptr && "failed to open file!!!\n");

    fprintf(output_file, "stackDump called from %s: function %s: %d\n", f_data->file_name,
                                                                        f_data->func_name,
                                                                        f_data->line);
    fprintf(output_file, "error code: ");
    decimalToBinary(GLOBAL_ERROR, output_file);

    /*
        for (int index = 0; index < sizeof(errors); index++)
        {
            if (errors[i].flag)
            {
                fprintf(output_file, "%dth error:\n", index);
                fprintf(output_file, "%s\n", errors[index].description);
            }
        }
    */

    if (stack != nullptr)
    {
        fprintf(output_file, "stack1 [%p]\n",    stack);
    }
    else
    {
        fprintf(output_file, "stack1 !!!![nullptr]!!!!\n");
        return;
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
        fprintf(output_file, "}\n");
        fprintf(output_file, "chini svou programmy\n");
        return;
    }
    fprintf(output_file, "\n");
    if (stack->data != nullptr)
    {
        if (stack->size >= 0 && stack->size <= stack->capasity)
        {
            fprintf(output_file, "Left canary = %d\t(should be %d)\n",
                    stack->data[canaries.ind_l_can], canaries.val_l_can);
            for (stack_size_t index = 1; index <= stack->size; index++)
            {
                fprintf(output_file, "\t*[%lld] = %d\n", index, stack->data[index]);
            }
            for (stack_capasity_t index_poizon = (stack_capasity_t)(stack->size + 1);
                index_poizon <= stack->capasity; index_poizon++)
            {
                fprintf(output_file, "\t [%zd] = %d (TRASH)\n", index_poizon,
                                                                 stack->data[index_poizon]);
            }
            fprintf(output_file, "Right canary = %d\t(should be %d)\n",
                    stack->data[canaries.ind_r_can], canaries.val_r_can);
        }
        else if (stack->capasity > 0)
        {
            for (stack_capasity_t index_poizon = 1;
                index_poizon <= stack->capasity; index_poizon++)
            {
                fprintf(output_file, "\t [%zd] = %d (TRASH)\n", index_poizon,
                                                                 stack->data[index_poizon]);
            }
        }
    }
    fprintf(output_file, "} \n");
    fclose(output_file);

    return;
}

int_error_t addElements (my_stack_t* stack)
{
    int_error_t  code_error = CHECK_STACK;

    for (stack_elem_t index = 0; index < stack->size; index++)
    {
        stack->data[index] = index * 10;
    }

    code_error = CHECK_STACK

    return HAVE_NO_ERRORS;
}
