#include "operationsCalc.h"

#include <math.h>

int_error_t stackAdd  (my_stack_t* stack)
{
    int_error_t  code_error = CHECK_STACK;

    stack_elem_t number1 = 0;
    stack_elem_t number2 = 0;
    stackPop(stack, &number1);
    stackPop(stack, &number2);
    stack_elem_t add = number1 + number2;
    stackPush(stack, add);

    code_error = CHECK_STACK

    return HAVE_NO_ERRORS;
}
int_error_t stackSub  (my_stack_t* stack)
{
    int_error_t  code_error = CHECK_STACK;

    stack_elem_t number1 = 0;
    stack_elem_t number2 = 0;
    stackPop(stack, &number1);
    stackPop(stack, &number2);
    stack_elem_t sub = number2 - number1;
    stackPush(stack, sub);

    code_error = CHECK_STACK

    return HAVE_NO_ERRORS;
}
int_error_t stackMul  (my_stack_t* stack)
{
    int_error_t  code_error = CHECK_STACK;

    stack_elem_t number1 = 0;
    stack_elem_t number2 = 0;
    stackPop(stack, &number1);
    stackPop(stack, &number2);
    stack_elem_t mul = number1 * number2;
    stackPush(stack, mul);

    code_error = CHECK_STACK

    return HAVE_NO_ERRORS;
}
int_error_t stackDiv  (my_stack_t* stack)
{
    int_error_t  code_error = CHECK_STACK;

    stack_elem_t number1 = 0;
    stack_elem_t number2 = 0;
    stackPop(stack, &number1);
    stackPop(stack, &number2);
    if (number1 == 0)
    {
        stackPush(stack, number2);
        stackPush(stack, number1);
        fprintf(stderr, "Division by zero error\n");
        return ERR_DIV_BY_0;
    }
    stack_elem_t div = number2 / number1;
    stackPush(stack, div);

    code_error = CHECK_STACK

    return HAVE_NO_ERRORS;
}
int_error_t stackSqrt (my_stack_t* stack)
{
    int_error_t  code_error = CHECK_STACK;

    stack_elem_t number = 0;
    stackPop(stack, &number);
    number = (stack_elem_t)sqrt(number);
    stackPush(stack, number);

    code_error = CHECK_STACK

    return HAVE_NO_ERRORS;
}

