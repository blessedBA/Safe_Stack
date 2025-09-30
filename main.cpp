#include "calculator.h"
#include "stackFuncs.h"
#include "safetyOfStack.h"


#include <assert.h>
#include <stdio.h>



void crashProgramm (my_stack_t* stack);

int main()
{
    //stack_elem_t number = 5;
    //stack_elem_t value = 0;
    my_stack_t stack1 = {};
    stackInit(&stack1);
    calculatorOn(&stack1);
    //crashProgramm(&stack1);
    // stack1.data = nullptr;
    // stack1.size = -5;
    // stack1.capasity = -10;

    //stackPush(&stack1, number++);
    // stackPush(&stack1, number++);
    // stackPush(&stack1, number++);
    // stackPush(&stack1, number++);
    // stackPush(&stack1, number++);
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
            stack->capasity = -10;
            break;
        case ERR_SIZE_MORE_CAPASITY:
            stack->size = -5;
            stack->capasity = -10;
            break;
        case ERR_INVALID_PTR_TO_ELEM:
            stack->data[1] = BAD_VALUE;
            break;
        case ERR_LEFT_ATTACK:
            ;
            break;
        case ERR_RIGHT_ATTACK:
            ;
            break;
        default:
            assert(0 && "invalid code of error!!!\n");
    }

    return;
}


