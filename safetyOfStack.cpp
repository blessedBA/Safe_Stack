#include "safetyOfStack.h"
#include "stackFuncs.h"
#include "Files.h"

#include <assert.h>

#define PR_SIZE_AND_CAP fprintf(output_file, "{ \n");                                 \
                        fprintf(output_file, "\t size     = %lld\n", stack->size);    \
                        fprintf(output_file, "\t capacity = %zd\n", stack->capacity);

#define PR_NULLED_DATA  fprintf(output_file, "\t data !!!![nullptr]!!!!\n"); \
                        fprintf(output_file, "}\n");                         \
                        fprintf(output_file, "chini svou programmy\n");      \

extern canary_t canaries;

storageErrors errors[] = { {HAVE_NO_ERRORS,          code_HAVE_NO_ERRORS,          "you have no errors!!!",                    true },
                           {ERR_INVALID_DATA,        code_ERR_INVALID_DATA,        "data have invalid pointer to 1st element", false},
                           {ERR_INVALID_SIZE,        code_ERR_INVALID_SIZE,        "size of stack is negative",                false},
                           {ERR_INVALID_CAPASITY,    code_ERR_INVALID_CAPASITY,    "capacity of stack is negative",            false},
                           {ERR_SIZE_MORE_CAPASITY,  code_ERR_SIZE_MORE_CAPASITY,  "size is more than capacity",               false},
                           {ERR_INVALID_PTR_TO_ELEM, code_ERR_INVALID_PTR_TO_ELEM, "invalid pointer to element",               false},
                           {ERR_LEFT_ATTACK,         code_ERR_LEFT_ATTACK,         "stack attacted from left side",            false},
                           {ERR_RIGHT_ATTACK,        code_ERR_RIGHT_ATTACK,        "stack attacted from right side",           false},
                           {ERR_DIV_BY_0,            code_ERR_DIV_BY_0,            "division by zero",                         false}  };
int_error_t global_code_error = code_HAVE_NO_ERRORS;
int_error_t stackVerify (my_stack_t* stack, const char* file_n,
                                            const char* func_n, int line_str)
{
    func_data f_data = { file_n, func_n, line_str };
    if (stack->data == nullptr)
    {
        errors[HAVE_NO_ERRORS].isError = false;
        errors[ERR_INVALID_DATA].isError = true;
    }
    if (stack->size < 0)
    {
        errors[HAVE_NO_ERRORS].isError = false;
        errors[ERR_INVALID_SIZE].isError = true;
    }
    if (stack->capacity < 0)
    {
        errors[HAVE_NO_ERRORS].isError = false;
        errors[ERR_INVALID_CAPASITY].isError = true;
    }
    if (stack->size > stack->capacity)
    {
        errors[HAVE_NO_ERRORS].isError = false;
        errors[ERR_SIZE_MORE_CAPASITY].isError = true;
    }
    if (stack->data != nullptr && stack->capacity >= stack->size && stack->size >= 0)
    {
        for (stack_capasity_t index = 1; index < stack->capacity; index++)
        {
            if (stack->data + index == nullptr)
            {
                errors[HAVE_NO_ERRORS].isError = false;
                errors[ERR_INVALID_PTR_TO_ELEM].isError = true;
                break;
            }
        }
    }
    if (stack->capacity > 0 &&
        stack->data != nullptr && (stack->data + stack->capacity + 1) != nullptr)
    {
        if (stack->data[canaries.ind_l_can] != canaries.val_l_can)
        {
            errors[HAVE_NO_ERRORS].isError = false;
            errors[ERR_LEFT_ATTACK].isError = true;
        }
        if (stack->data[canaries.ind_r_can] != canaries.val_r_can)
        {
            errors[HAVE_NO_ERRORS].isError = false;
            errors[ERR_RIGHT_ATTACK].isError = true;
        }
    }
    if (errors[HAVE_NO_ERRORS].isError == false)
    {
        stackDump (stack, global_code_error, &f_data);
    }

    return global_code_error; // возвращать код глобальной ошибки(двоичное число)
}

bool setCanaries (my_stack_t* stack)
{
    assert(stack != nullptr);
    if (stack->capacity <= 0 && stack->data == nullptr)
    {
        return false;
    }
    canaries.ind_r_can = stack->capacity + 1;
    stack->data[canaries.ind_l_can] = canaries.val_l_can;
    stack->data[canaries.ind_r_can] = canaries.val_r_can;

    return true;
}

void stackDump (my_stack_t* stack, int GLOBAL_ERROR, func_data* f_data)
{
    printf("stackDump was called!\n");
    FILE* output_file = fopen(LOG_FILE, "a");
    assert(output_file != nullptr && "failed to open file!!!\n");

    fprintf(output_file, "stackDump called from %s: function %s: %d\n", f_data->file_name,
                                                                        f_data->func_name,
                                                                        f_data->line);
    size_t size_st_errors = sizeof(errors) / sizeof(errors[0]);
    for (size_t index = 0; index < size_st_errors; index++)
    {
        if (errors[index].isError)
        {
            fprintf(output_file, "Errors:\n%s\n", errors[index].description);
            GLOBAL_ERROR |= errors[index].code_error;
        }
    }
    fprintf(output_file, "error code: ");
    decimalToBinary(GLOBAL_ERROR, output_file);
    if (stack != nullptr)
    {
        fprintf(output_file, "stack1 [%p]\n",    stack);
    }
    else
    {
        fprintf(output_file, "stack1 !!!![nullptr]!!!!\n");
        return;
    }
    PR_SIZE_AND_CAP
    if (stack->data != nullptr)
    {
        fprintf(output_file, "\t data [%p]\n", stack->data);
    }
    else
    {

        return;
    }
    fprintf(output_file, "\n");
    if (stack->data != nullptr)
    {
        if (stack->size >= 0 && stack->size <= stack->capacity)
        {
            fprintf(output_file, "Left canary = %d\t(should be %d)\n",
                    stack->data[canaries.ind_l_can], canaries.val_l_can);
            printSizeAndCapacity(stack, output_file);
            fprintf(output_file, "Right canary = %d\t(should be %d)\n",
                    stack->data[canaries.ind_r_can], canaries.val_r_can);
        }
        else if (stack->capacity > 0)
        {
            for (stack_capasity_t index_poizon = 1;
                index_poizon <= stack->capacity; index_poizon++)
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

void printSizeAndCapacity (my_stack_t* stack, FILE* output_file)
{
    for (stack_size_t index = 1; index <= stack->size; index++)
    {
        fprintf(output_file, "\t*[%lld] = %d\n", index, stack->data[index]);
    }
    for (stack_capasity_t index_poizon = (stack_capasity_t)(stack->size + 1);
        index_poizon <= stack->capacity; index_poizon++)
    {
        fprintf(output_file, "\t [%zd] = %d (TRASH)\n", index_poizon,
                                                            stack->data[index_poizon]);
    }

    return;
}
