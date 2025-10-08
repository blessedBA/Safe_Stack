#include "buffer.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void writeBuffer (char* buffer, FILE* file_input, ssize_t* size_file)
{
    assert(buffer != nullptr && file_input != nullptr);
    assert(file_input != nullptr && "invalid pointer to file_input!!!\n");
    assert(size_file != nullptr && "invalid pointer to size_file!!!\n");

    *size_file = (ssize_t)(fread(buffer, sizeof(char), (size_t)*size_file, file_input) + 1);
    if (*size_file == 0)
    {
        assert(0 && "failed to read file!!!\n");
    }
    addEndOfStringToEOF(buffer, *size_file);
}
char* creatBuffer(ssize_t* size_file, const char* filepath)
{

    assert(size_file != nullptr && "invalid pointer to size_file!!!\n");
    assert(filepath != nullptr && "invalid pointer to filepath!!!\n");

    struct stat st = {};
    const int get_info = (int)stat(filepath, &st);
    *size_file = st.st_size;
    if (get_info != 0)
    {
        perror("failed to get info about file!!!\n");
        return nullptr;
    }
    char*  buffer = (char*)calloc((size_t)(*size_file + 2), sizeof(char));
    return buffer;
}

void addEndOfStringToEOF (char* buffer, const ssize_t size_file)
{
    assert(buffer != nullptr && "invalid pointer to buffer!!!\n");

    buffer[size_file - 1] = EOF;
    buffer[size_file - 2] = '\0';
    return;
}

