#include <slug_rw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RW_LINE_BUFFER_SIZE 1024

static SourceFile* slug_init_rw() {
    SourceFile* file = (SourceFile*) malloc(sizeof(SourceFile));
    file->path = (char**) malloc(sizeof(char*));
    file->size = 0;
    file->capacity = 1;
    return file;
}


SourceFile* slug_rw_read_file(char* path) {
    SourceFile* file = slug_init_rw();
    char line[RW_LINE_BUFFER_SIZE] = {0};
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", path);
        exit(1);
    }
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (file->size == file->capacity) {
            file->capacity *= 2;
            file->path = realloc(file->path, file->capacity * sizeof(char*));
        }
        file->path[file->size++] = strdup(line);
    }
    fclose(fp);
    return file;
}

void slug_rw_free(SourceFile* file) {
    for (int i = 0; (size_t) i < file->size; i++) {
        free(file->path[i]);
    }
    free(file->path);
    free(file);
}