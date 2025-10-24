#include "Utils_asm.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#ifndef N_DEBUG
#define N_DEBUG 1L
#endif


void skipSpaces(FILE* input_file)
{
    #ifndef NDEBUG
    assert(input_file);
    #endif

    int ch = fgetc(input_file);
    while (ch != EOF && isspace(ch))
        ch = fgetc(input_file);
    if (ch != EOF) ungetc(ch, input_file);

    #ifndef NDEBUG
    assert(input_file);
    #endif

    return;
}

void skipSpacesNoNewline(FILE* input_file)
{
    #ifndef NDEBUG
    assert(input_file);
    #endif

    int ch = fgetc(input_file);
    while (ch != EOF && (ch == ' ' || ch == '\t' || ch == '\r'))
    {
        ch = fgetc(input_file);
    }
    if (ch != EOF)
    {
        ungetc(ch, input_file);
    }

    #ifndef NDEBUG
    assert(input_file);
    #endif

    return;
}

void skipString (FILE* asm_file)
{
    #ifndef NDEBUG
    assert(asm_file);
    #endif

    int ch = 0;
    while((ch = fgetc(asm_file)) != EOF && ch != '\n')
    ;

    #ifndef NDEBUG
    assert(asm_file);
    #endif

    return;
}

