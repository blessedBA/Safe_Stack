#ifndef OPERATIONS_CALC_H
#define OPERATIONS_CALC_H

#include "safetyOfStack.h"
#include "stackFuncs.h"

int_error_t stackAdd  (my_stack_t* stack);
int_error_t stackSub  (my_stack_t* stack);
int_error_t stackMul  (my_stack_t* stack);
int_error_t stackDiv  (my_stack_t* stack);
int_error_t stackSqrt (my_stack_t* stack);


#endif // OPERATIONS_CALC_H
