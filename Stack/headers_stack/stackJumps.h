#ifndef STACK_JUMPS_H
#define STACK_JUMPS_H

#include "generalObjectForStack.h"
#include "Processor/processor_PASH.h"

void stackJMP (Processor_t* proc, int arg);
void stackJB (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num);
void stackJBE (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num);
void stackJA (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num);
void stackJAE (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num);
void stackJE (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num);
void stackJNE (Processor_t* proc, int arg, stack_elem_t* prelast_num, stack_elem_t* last_num);

#endif // STACK_JUMPS_H
