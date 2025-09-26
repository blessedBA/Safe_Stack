#ifndef SAFETY_OF_STACK_H
#define SAFETY_OF_STACK_H

typedef struct
{
    const char* file_name;
    const char* func_name;
    const int line;
} func_data;

typedef int int_error_t;
typedef enum
{
    HAVE_NO_ERRORS = 0,
    ERR_INVALID_DATA = 1,
    ERR_INVALID_SIZE = 2,
    ERR_INVALID_CAPASITY = 3,
    ERR_SIZE_MORE_CAPASITY = 4,
    ERR_INVALID_PTR_TO_ELEM = 5

} my_error_t;

typedef enum
{
    code_HAVE_NO_ERRORS          = 0,
    code_ERR_INVALID_DATA        = 1,
    code_ERR_INVALID_SIZE        = 2,
    code_ERR_INVALID_CAPASITY    = 4,
    code_ERR_SIZE_MORE_CAPASITY  = 8,
    code_ERR_INVALID_PTR_TO_ELEM = 16

} my_error_code_t;

#endif // SAFETY_OF_STACK_H
