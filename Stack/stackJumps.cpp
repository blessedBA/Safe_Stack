#include "stackJumps.h"
#include "stackFuncs.h"
#include "generalObjectForStack.h"
#include "../Processor/processor_PASH.h"

void stackJMP (Processor_t* proc, int arg)
{
    proc->P_C = (size_t)arg;

    return;
}

void stackJB (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    stackPop(&proc->stack1, prelast_num);
    stackPop(&proc->stack1, last_num);
    (prelast_num < last_num) ? proc->P_C = (size_t)arg : proc->P_C++;

    return;
}

void stackJBE (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    stackPop(&proc->stack1, prelast_num);
    stackPop(&proc->stack1, last_num);
    (prelast_num <= last_num) ? proc->P_C = (size_t)arg : proc->P_C++;

    return;
}

void stackJA (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    stackPop(&proc->stack1, prelast_num);
    stackPop(&proc->stack1, last_num);
    (prelast_num > last_num) ? proc->P_C = (size_t)arg : proc->P_C++;

    return;
}

void stackJAE (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    stackPop(&proc->stack1, prelast_num);
    stackPop(&proc->stack1, last_num);
    (prelast_num >= last_num) ? proc->P_C = (size_t)arg : proc->P_C++;

    return;
}

void stackJE (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    stackPop(&proc->stack1, prelast_num);
    stackPop(&proc->stack1, last_num);
    (prelast_num == last_num) ? proc->P_C = (size_t)arg : proc->P_C++;

    return;
}

void stackJNE (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num)
{
    stackPop(&proc->stack1, prelast_num);
    stackPop(&proc->stack1, last_num);
    (prelast_num != last_num) ? proc->P_C = (size_t)arg : proc->P_C++;

    return;
}


