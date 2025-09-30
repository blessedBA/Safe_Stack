#ifndef SAFETY_OF_STACK_H
#define SAFETY_OF_STACK_H
#define CHECK_STACK stackVerify(stack, __FILE__, __func__, __LINE__); \
                    if (code_error) \
                    { \
                        return code_error; \
                    }

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
    code_ERR_RIGHT_ATTACK        = 64


} my_error_code_t;

//typedef struct ...

#endif // SAFETY_OF_STACK_H
