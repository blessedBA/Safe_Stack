#ifndef SAFETY_OF_STACK_H
#define SAFETY_OF_STACK_H
#define CHECK_STACK stackVerify(stack, __FILE__, __func__, __LINE__); \
                    if (code_error) \
                    { \
                        return code_error; \
                    }
#include "generalObjectForStack.h"
#define N_DEBUG 1L

typedef enum
{
    HAVE_NO_ERRORS = 0,
    ERR_INVALID_DATA = 1,
    ERR_INVALID_SIZE = 2,
    ERR_INVALID_CAPASITY = 3,
    ERR_SIZE_MORE_CAPASITY = 4,
    ERR_INVALID_PTR_TO_ELEM = 5,
    ERR_LEFT_ATTACK = 6,
    ERR_RIGHT_ATTACK = 7,

    ERR_DIV_BY_0 = 8

} my_error_t;

typedef enum
{
    code_HAVE_NO_ERRORS          = 0,
    code_ERR_INVALID_DATA        = 1,
    code_ERR_INVALID_SIZE        = 2,
    code_ERR_INVALID_CAPASITY    = 4,
    code_ERR_SIZE_MORE_CAPASITY  = 8,
    code_ERR_INVALID_PTR_TO_ELEM = 16,
    code_ERR_LEFT_ATTACK         = 32,
    code_ERR_RIGHT_ATTACK        = 64,

    code_ERR_DIV_BY_0            = 128

} my_error_code_t;

typedef struct
{
    const my_error_t error;
    const my_error_code_t code_error;
    const char* description;
    bool isError;
} storageErrors;

void stackDump (my_stack_t* stack, int GLOBAL_ERROR, func_data* f_data);
int_error_t stackVerify  (my_stack_t* stack, const char* file_n,
                                                 const char* func_n, int line_str);
bool setCanaries (my_stack_t* stack);
void printSizeAndCapacity (my_stack_t* stack, FILE* output_file);


#endif // SAFETY_OF_STACK_H
