#ifndef TRANSLATOR_H
#define TRANSLATOR_H

typedef enum
{
    HAVE_ERROR = -1,
    HAVE_NO_ARG = 0,
    HAVE_ARG = 1
} isArg;

typedef struct
{

} 

bool creatByteCode (char* buffer, FILE* output_file, ssize_t size_file);
isArg getCodeCommand (commandStorage* command, char* name_command);


#endif // TRANSLATOR_H
