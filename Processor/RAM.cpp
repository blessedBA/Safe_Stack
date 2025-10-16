#include "RAM.h"

#include <stdio.h>
#include <stdlib.h>
ram_t* createRAM(size_t size_RAM)
{
    ram_t* ram = (ram_t*)calloc(size_RAM, sizeof(ram_t));

    return ram;
}
