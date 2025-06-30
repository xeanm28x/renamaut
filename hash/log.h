// log.h
#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define LOG_DEBUG(fmt, ...) \
    do { \
        FILE* f = fopen("debug.log", "a"); \
        if (f) { \
            fprintf(f, "[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
            fclose(f); \
        } \
    } while (0)

#endif
