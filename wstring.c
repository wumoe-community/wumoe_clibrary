#include "wstring.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

char *__concat__(size_t argc, char** argv) {
    size_t total_len = 1, len[argc];
    for (size_t i = 0; i < argc; ++i)
        total_len += (len[i] = strlen(argv[i]));
    char* dest = malloc(total_len * sizeof(char));
    for (size_t i = 0, offset = 0; i < argc; offset += len[i], ++i)
        strcpy(dest + offset, argv[i]);
    return dest;
}
