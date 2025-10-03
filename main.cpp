#include "calculator.h"
#include "stackFuncs.h"
#include "safetyOfStack.h"
#include "generalObjectForStack.h"

#include <assert.h>
#include <stdio.h>

#define TRY_MY_STACK stack1.size = -5;                  \
                     stack1.capacity = -10;             \
                     stack1.data[0] = 52;

void crashProgramm (my_stack_t* stack);

int main()
{
    my_stack_t stack1 = {};
    stackInit(&stack1);

    calculatorOn(&stack1);
    //crashProgramm(&stack1);
    //stack1.data[0] = 52;
    //printf("%d\n", stack1.data[0]);
    stackDestroy(&stack1);
    return 0;
}

void crashProgramm (my_stack_t* stack)
{
    switch (ERR_INVALID_DATA)
    {
        case HAVE_NO_ERRORS:
            break;
        case ERR_INVALID_DATA:
            stack->data = nullptr;
            break;
        case ERR_INVALID_SIZE:
            stack->size = -5;
            break;
        case ERR_INVALID_CAPASITY:
            stack->capacity = -10;
            break;
        case ERR_SIZE_MORE_CAPASITY:
            stack->size = -5;
            stack->capacity = -10;
            break;
        case ERR_INVALID_PTR_TO_ELEM:
            stack->data[1] = BAD_VALUE;
            break;
        case ERR_LEFT_ATTACK:
            stack->data[0] = 52;
            break;
        case ERR_RIGHT_ATTACK:
            stack->data[stack->capacity] = 42;
            break;
        default:
            assert(0 && "invalid code of error!!!\n");
    }

    return;
}


