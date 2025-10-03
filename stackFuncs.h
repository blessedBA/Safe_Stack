#ifndef STACK_H
#define STACK_H
#include "generalObjectForStack.h"


#include <stdio.h>

const int BAD_VALUE = -666;

typedef struct
{
    stack_capasity_t ind_l_can;
    stack_capasity_t ind_r_can;
    stack_elem_t     val_l_can;
    stack_elem_t     val_r_can;
} canary_t;


int_error_t stackInit    (my_stack_t* stack);
int_error_t stackPush    (my_stack_t* stack, stack_elem_t number);
int_error_t stackPop     (my_stack_t* stack, stack_elem_t* value);
int_error_t stackDestroy (my_stack_t* stack);
int_error_t printStack   (my_stack_t* stack, FILE* output_file);


int_error_t addElements (my_stack_t* stack);
void decimalToBinary(int decimalNum, FILE* output_file);


#endif // STACK_H
