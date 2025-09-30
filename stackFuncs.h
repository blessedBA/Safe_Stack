#ifndef STACK_H
#define STACK_H
#include "safetyOfStack.h"


#include <stdio.h>

const int BAD_VALUE = -666;
typedef int stack_elem_t;
typedef ssize_t stack_capasity_t;
typedef long long stack_size_t;



typedef struct
{
    stack_elem_t*    data;
    stack_capasity_t capasity;
    stack_size_t     size;
} my_stack_t;

typedef struct
{
    stack_capasity_t ind_l_can;
    stack_capasity_t ind_r_can;
    stack_elem_t     val_l_can;
    stack_elem_t     val_r_can;
} canary_t;

int_error_t stackVerify  (my_stack_t* stack, const char* file_n,
                                                 const char* func_n, int line_str);

int_error_t stackInit    (my_stack_t* stack);
int_error_t stackPush    (my_stack_t* stack, stack_elem_t number);
int_error_t stackPop     (my_stack_t* stack, stack_elem_t* value);
int_error_t stackDestroy (my_stack_t* stack);
int_error_t printStack   (my_stack_t* stack, FILE* output_file);
void stackDump (my_stack_t* stack, int GLOBAL_ERROR, func_data* f_data);
bool setCanaries (my_stack_t* stack);
int_error_t addElements (my_stack_t* stack);
void decimalToBinary(int decimalNum, FILE* output_file);





#endif // STACK_H
