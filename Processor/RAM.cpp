#include "processor_PASH.h"
#include "RAM.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//#define NDEBUG
#define SIZE_RAM    2500
#define WIDTH_RAM   50
#define COUNT_LINES 50

void dumpRAM (Processor_t* processor);

ram_t* createRAM(size_t size_RAM)
{
    printf("size_RAM = %zu\n", size_RAM);
    ram_t* ram = (ram_t*)calloc(size_RAM, sizeof(ram_t));
    printf("ram = %p\n", ram);
    return ram;
}

void dumpRAM (Processor_t* processor)
{
    #ifndef NDEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif
    int ind = 0;
    FILE* output_file = stdout;
    fprintf(output_file, "----------------------------------------DumpRAM-------------------------\n\n");

    for (int index = 0; index < COUNT_LINES; index++) // in the begining of each string print index string
    {
        fprintf(output_file, "[%d][]  ", index);
        for ( ; ind < SIZE_RAM; ind++)
        {
            if (ind % WIDTH_RAM == 0 && ind != 0 && ind / WIDTH_RAM == index + 1) break;
            fprintf(output_file, "%c", processor->RAM[ind]);
        }
        fprintf(output_file, "\n");
    }

    fprintf(output_file, "\n--------------------------------------End-of-DumpRAM-----------------\n");

    #ifndef NDEBUG
    procVerify(processor, __FILE__, __func__, __LINE__);
    #endif

    return;
}
