#include "calculator.h"
#include "generalObjectForStack.h"
#include "Translator.h"
#include "stackFuncs.h"
#include <ctype.h>

#define MAX_COUNT_ERR 20
bool creatByteCode (char* buffer, FILE* output_file, ssize_t size_file)
{
    func_data error_lines[MAX_COUNT_ERR] = {};
    commandStorage commands = {};
    char command[5] = "";
    int arg[1000] = {};
    int ind_arg = 0;
    int ind_comm = 0;
    int line = 0;
    int ind_error = 0;
    for (ssize_t index = 0; index < size_file; index++)
    {
        line++;
        while(buffer[index] != ' ')
        {
            command[ind_comm] = buffer[index];
            index++;
        }
        switch (getCodeCommand(&commands, command))
        {
            case HAVE_ERROR:
                error_lines[ind_error].file_name = __FILE__;
                error_lines[ind_error].func_name = __func__;
                error_lines[ind_error].line      = line;
                ind_error++;
                break;
            case HAVE_NO_ARG:
                


        }
        if (getCodeCommand(&commands, command) == HAVE_ARG)
        {
            index++;
            while(isalnum(buffer[index]))
            {
                arg[ind_arg] = buffer[index];
                ind_arg++;
                index++;
            }
            arg[ind_arg] = BAD_VALUE;
        }


    }
}

isArg getCodeCommand (commandStorage* command, char* name_command)
{
    if (strcmp(command->PUSH, name_command) == 0)
    {
        *code_command = code_PUSH;
        return HAVE_ARG;
    }
    else if (strcmp(command->POP, name_command) == 0)
    {
        *code_command = code_POP;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->OUT, name_command) == 0)
    {
        *code_command = code_OUT;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->ADD, name_command) == 0)
    {
        *code_command = code_ADD;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->SUB, name_command) == 0)
    {
        *code_command = code_SUB;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->MUL, name_command) == 0)
    {
        *code_command = code_MUL;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->DIV, name_command) == 0)
    {
        *code_command = code_DIV;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->SQRT, name_command) == 0)
    {
        *code_command = code_SQRT;
        return HAVE_NO_ARG;
    }
    else if (strcmp(command->HLT, name_command) == 0)
    {
        *code_command = code_HLT;
        return HAVE_NO_ARG;
    }
    else
    {
        return HAVE_ERROR;
    }

}


