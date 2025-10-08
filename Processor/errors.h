#ifndef ERRORS_H
#define ERRORS_H

typedef enum
{
    NO_ERRORS_PR   = 0,
    ERR_FAIL_OP_BT = 1,
    ERR_READ_BT    = 2,
    ERR_INV_PROC   = 3,
    ERR_INV_STACK  = 4,
    ERR_INV_B_CODE = 5
} error_pr_t;

typedef enum
{
    code_NO_ERRORS_PR   = 0,
    code_ERR_FAIL_OP_BT = 1 << 0,
    code_ERR_READ_BT    = 1 << 1,
    code_ERR_INV_PROC   = 1 << 2,
    code_ERR_INV_CODE   = 1 << 3,
    code_ERR_INV_STACK  = 1 << 4,
    code_ERR_INV_B_CODE = 1 << 5

} error_pr_code;

typedef struct
{
    const error_pr_t error;
    const error_pr_code error_code;
    const char* description;
    bool isError;
} storageProcErrors;

#endif // ERRORS_H
