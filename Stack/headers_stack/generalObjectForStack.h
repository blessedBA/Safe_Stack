#ifndef GENERAL_OBJECT_FOR_STACK_H
#define GENERAL_OBJECT_FOR_STACK_H

#include <stdio.h>

typedef int stack_elem_t;
typedef ssize_t stack_capasity_t;
typedef long long stack_size_t;

typedef struct
{
    const char* file_name;
    const char* func_name;
    int line;
} func_data;

typedef int int_error_t;

typedef struct
{
    stack_elem_t*    data;
    stack_capasity_t capacity;
    stack_size_t     size;
} my_stack_t;



#endif // GENERAL_OBJECT_FOR_STACK_H
