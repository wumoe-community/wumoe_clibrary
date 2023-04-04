#ifndef WPM_WSTRING_H
#define WPM_WSTRING_H

#include <stddef.h>

#define concat(...) __concat__(sizeof((char*[]){__VA_ARGS__})/sizeof(char*),(char*[]){__VA_ARGS__})

char *__concat__(size_t argc, char** argv);

#endif
