#include <core/memory/alloc.h>

#ifndef STDLIB_H
#define STDLIB_H

#include <cuteng/ce-libc/stdlib.h>
#include <cuteng/ce-libc/string.h>
#include <core/memory/alloc.h>

namespace std {
    void reverse(char str[], int length);
    void itoa(int num, char str[], int base);
    char *strdup(const char *s1);
}


#endif