#include "safetyOfStack.h"
#include "stackFuncs.h"
#include "Files.h"

int_error_t global_code_error = code_HAVE_NO_ERRORS;
int_error_t stackVerify (my_stack_t* stack, const char* file_n,
                                            const char* func_n, int line_str)
{
    func_data f_data = { file_n, func_n, line_str };
    if (stack->data == nullptr)
    {
        global_code_error |= code_ERR_INVALID_DATA;
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
    if (global_code_error != 0)
    {
        stackDump (stack, global_code_error, &f_data);
    }

    return global_code_error;
}
