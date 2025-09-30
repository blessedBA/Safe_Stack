#include "safetyOfStack.h"
#include "stackFuncs.h"
#include "Files.h"

#include <assert.h>

extern canary_t canaries;

int_error_t global_code_error = code_HAVE_NO_ERRORS;
int_error_t stackVerify (my_stack_t* stack, const char* file_n,
                                            const char* func_n, int line_str)
{
    func_data f_data = { file_n, func_n, line_str };
    if (stack->data == nullptr)
    {

        global_code_error |= code_ERR_INVALID_DATA;
        // errors[ERR_INVALID_DATA].flag = true;
    }
    if (stack->size < 0)
    {
        global_code_error |= code_ERR_INVALID_SIZE;
    }
    if (stack->capasity < 0)
    {
        global_code_error |= code_ERR_INVALID_CAPASITY;
    }
    if (stack->size > stack->capasity)
    {
        global_code_error |= code_ERR_SIZE_MORE_CAPASITY;
    }
    if (stack->data != nullptr && stack->capasity >= stack->size && stack->size >= 0)
    {
        for (stack_capasity_t index = 1; index < stack->capasity; index++)
        {
            if (stack->data + index == nullptr)
            {
                global_code_error |= code_ERR_INVALID_PTR_TO_ELEM;
                break;
            }
        }
    }
    if (stack->capasity > 0 &&
        stack->data != nullptr && (stack->data + stack->capasity + 1) != nullptr)
    {
        if (stack->data[canaries.ind_l_can] != canaries.val_l_can)
        {
            global_code_error |= code_ERR_LEFT_ATTACK;
        }
        if (stack->data[canaries.ind_r_can] != canaries.val_r_can)
        {
            global_code_error |= code_ERR_RIGHT_ATTACK;
        }
    }
    if (global_code_error != 0)
    {
        stackDump (stack, global_code_error, &f_data);
    }

    return global_code_error;
}

bool setCanaries (my_stack_t* stack)
{
    assert(stack != nullptr);
    if (stack->capasity <= 0 && stack->data == nullptr)
    {
        return false;
    }
    canaries.ind_r_can = stack->capasity + 1;
    stack->data[canaries.ind_l_can] = canaries.val_l_can;
    stack->data[canaries.ind_r_can] = canaries.val_r_can;

    return true;
}
