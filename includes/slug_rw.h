#ifndef _SLUG_RW_H
#define _SLUG_RW_H
#include <stdio.h>

typedef struct {
    char** path;
    size_t size;
    size_t capacity;
} SourceFile;

SourceFile* slug_rw_read_file(char* path);
void slug_rw_free(SourceFile* file);
#endif // _SLUG_RW_H