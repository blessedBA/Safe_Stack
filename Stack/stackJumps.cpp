#include "stackJumps.h"
#include "stackFuncs.h"
#include "generalObjectForStack.h"
#include "../Processor/processor_PASH.h"
#include "../Assembler/assembler.h"

#include <assert.h>
#include <stdlib.h>

#define NUMBER_LABELS 10

extern label_struct labels[NUMBER_LABELS];

void stackJMP (Processor_t* proc, label_t arg)
{
    #if N_DEBUG
    assert(proc);
    #endif

    proc->P_C = (size_t)arg;

    #if N_DEBUG
    assert(proc);
    #endif

    return;
}

void stackJB (Processor_t* proc, label_t arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    stackPop(&proc->stack1, last_num);
    stackPop(&proc->stack1, prelast_num);
    if (*prelast_num > *last_num)
    {
        proc->P_C = (size_t)arg;
    }

    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    return;
}

void stackJBE (Processor_t* proc, label_t arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    stackPop(&proc->stack1, last_num);
    stackPop(&proc->stack1, prelast_num);
    if (*prelast_num >= *last_num)
    {
        proc->P_C = (size_t)arg;
    }

    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    return;
}

void stackJA (Processor_t* proc, label_t arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    stackPop(&proc->stack1, last_num);
    stackPop(&proc->stack1, prelast_num);
    if (*prelast_num < *last_num)
    {
        proc->P_C = (size_t)arg;
    }

    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    return;
}

void stackJAE (Processor_t* proc, label_t arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    stackPop(&proc->stack1, last_num);
    stackPop(&proc->stack1, prelast_num);
    if (*prelast_num <= *last_num)
    {
        proc->P_C = (size_t)arg;
    }

    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    return;
}

void stackJE (Processor_t* proc, label_t arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    stackPop(&proc->stack1, last_num);
    stackPop(&proc->stack1, prelast_num);
    if (*prelast_num == *last_num)
    {
        proc->P_C = (size_t)arg;
    }

    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    return;
}

void stackJNE (Processor_t* proc, label_t arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    stackPop(&proc->stack1, last_num);
    stackPop(&proc->stack1, prelast_num);
    if (*prelast_num != *last_num)
    {
        proc->P_C = (size_t)arg;
    }

    #if N_DEBUG
    assert(proc && prelast_num && last_num);
    #endif

    return;
}
