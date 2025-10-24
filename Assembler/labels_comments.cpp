#include "assembler.h"
#include "labels_comments.h"


#include <assert.h>
#include <stdio.h>
#include <ctype.h>

#define NUMBER_LABELS 10

extern label_struct labels[NUMBER_LABELS];
asm_error_t scanLabel (FILE* asm_file, size_t* count_elems, int* byte_code)
{
    #ifndef NDEBUG
    assert(asm_file && count_elems && byte_code);
    #endif

    //printf("count elems before scanLabel = %zu\n", *count_elems);
    label_t curr_label = 0;
    bool isReadLabel = false;
    int temp_count_elems = 0;
    if (fscanf(asm_file, "%d", &curr_label) != 1)
    {
        printf("error\n");
        return ERR_INV_LABEL_asm;
    }
    int ch = fgetc(asm_file);
    if (isspace(ch) == false)
    {
        return ERR_INV_LABEL_asm;
    }
    ungetc(ch, asm_file);
    for (int index = 0; index < NUMBER_LABELS; index++)
    {
        if (labels[index].isUsed && labels[index].isInit == false && labels[index].name == curr_label)
        {
            isReadLabel = true;
            labels[index].isInit = true;
            temp_count_elems = labels[index].adress_label;
            labels[index].adress_label = (int)*count_elems;
            byte_code[temp_count_elems] = (int)(*count_elems - 1);
            //printf("pizdec eto %d - label_name\n", curr_label);
            *count_elems += 1;
            break;
        }
        if (labels[index].isInit == false && labels[index].name == BAD_VALUE)
        {
            isReadLabel = true;
            labels[index].name = curr_label;
            labels[index].adress_label = (label_t)*count_elems;
            //printf("label_adress[%d] in scanLabel = %d\n", index, labels[index].adress_label);
            *count_elems += 1;
            labels[index].isInit = true;
            break;
        }
    }
    //printf("count elems after scanLabel = %zu\n", *count_elems);

    #ifndef NDEBUG
    assert(asm_file && count_elems && byte_code);
    #endif

    return (isReadLabel) ? HAVE_NO_ERRORS_asm : ERR_MAX_LABELS_asm;
}

void labelsVerify (int* byte_code, size_t count_elems)
{
    #ifndef NDEBUG
    assert(byte_code && count_elems != 0);
    #endif

    for (size_t index = 0; index < count_elems; index++)
    {
        if (byte_code[index] == code_JMP)
        {
            for (int ind = 0; ind < NUMBER_LABELS; ind++)
            {
                if (labels[ind].name == byte_code[index])
                {
                    if (labels[ind].isUsed == true && labels[ind].isInit == false)
                    {
                        fprintf(stderr, "invalid label on %zuth element\n", index);
                    }
                }
            }
        }
    }
    #ifndef NDEBUG
    assert(byte_code && count_elems != 0);
    #endif

    return;
}

bool scanComment (help_var_t* variables, FILE* asm_file)
{
    #ifndef NDEBUG
    assert(variables && asm_file);
    #endif

    skipSpacesNoNewline(asm_file);
    //fseek(asm_file, -1L, SEEK_CUR);
    int ch = fgetc(asm_file);
    if (ch == '\n' || ch == EOF)
    {
        printf("THERE WERE NOT COMMENT!!!\n\n");
        return false;
    }
    //ch = fgetc(asm_file);
    if (ch == ';' || variables->ch == ';')
    {
        printf("THAT WAS A COMMENT!!!\n\n");
        skipString(asm_file);
        return false;
    }
    ungetc(ch, asm_file);
    skipString(asm_file);

    #ifndef NDEBUG
    assert(variables && asm_file);
    #endif

    return true;
}

bool checkLabel (help_var_t* variables, FILE* asm_file, size_t* count_elems, int* byte_array, size_t count_lines, size_t* count_labels)
{
    #ifndef NDEBUG
    assert(variables && asm_file && byte_array && count_labels);
    #endif

    skipSpaces(asm_file);
    variables->ch = fgetc(asm_file);
    printf("variables->ch in checkLabel AFTER SKIPSPACES = %c\n", variables->ch);
    if (variables->ch == ':')
    {
        *count_labels += 1;
        switch (scanLabel(asm_file, count_elems, byte_array))
        {
            case HAVE_NO_ERRORS_asm:
                break;
            case ERR_INV_LABEL_asm:
                variables->flag_error = true;
                setError(count_lines, ERR_UNEXP_SYM_asm);
                variables->ind_error++;
            case ERR_MAX_LABELS_asm:
                variables->flag_error = true;
                setError(count_lines, ERR_UNEXP_SYM_asm);
                variables->ind_error++;
            default:
                fprintf(stderr, "assert called from %s:%d - invalid return value from %s\n", __FILE__, __LINE__, __func__);
                assert(0);
        }
        checkComment(variables, asm_file, count_lines);
        //skipString(asm_file);
        return true;
    }
    ungetc(variables->ch, asm_file);
    skipSpaces(asm_file);
    //checkComment(variables, asm_file, count_lines);

    #ifndef NDEBUG
    assert(variables && asm_file && byte_array && count_labels);
    #endif

    return false;
}

void checkComment (help_var_t* variables, FILE* asm_file, size_t count_lines)
{
    #ifndef NDEBUG
    assert(variables && asm_file);
    #endif

    if (scanComment(variables, asm_file))
    {
        //printf("scanComment was true!!!!!\n");
        variables->flag_error = true;
        setError(count_lines, ERR_UNEXP_SYM_asm);
        variables->ind_error++;

    }

    #ifndef NDEBUG
    assert(variables && asm_file);
    #endif

    return;
}
