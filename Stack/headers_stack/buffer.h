#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <sys/types.h>

void writeBuffer (char* buffer, FILE* file_input, ssize_t* size_file);
char* creatBuffer(ssize_t* size_file, const char* filepath);
void addEndOfStringToEOF (char* buffer, const ssize_t size_file);


#endif // BUFFER_H
