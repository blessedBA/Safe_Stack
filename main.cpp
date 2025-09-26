#include "stackFuncs.h"
#include "safetyOfStack.h"

#include <assert.h>
#include <stdio.h>



#define CHECK_STACK code_error = stackVerify(stack, __FILE__, __func__, __LINE__); \
                    if (code_error > 0) \
                    {  \
                        return code_error; \
                    }

int main()
{
    stack_elem_t number = 5;
    stack_elem_t value = 0;
    my_stack_t stack1 = {};
    stackInit(&stack1);

    switch (ERR_INVALID_PTR_TO_ELEM)
    {
        case HAVE_NO_ERRORS:
            break;
        case ERR_INVALID_DATA:
            stack1.data = nullptr;
            break;
        case ERR_INVALID_SIZE:
            stack1.size = -5;
            break;
        case ERR_INVALID_CAPASITY:
            stack1.capasity = -10;
            break;
        case ERR_SIZE_MORE_CAPASITY:
            stack1.size = -5;
            stack1.capasity = -10;
            break;
        case ERR_INVALID_PTR_TO_ELEM:
            stack1.data[1] = BAD_VALUE;
            break;
        default:
            assert(0 && "invalid code of error!!!\n");

    }

    // stackPush(&stack1, number++);
    // stackPush(&stack1, number++);
    // stackPush(&stack1, number++);
    // stackPush(&stack1, number++);
    // stackPush(&stack1, number++);
    printStack(&stack1);
    for (stack_size_t index = 0; index < stack1.size; index++)
    {
        stackPop(&stack1, &value);
        printf("value%lld = %d\n", index, value);
    }
    printStack(&stack1);
    stackDestroy(&stack1);
    return 0;
}


