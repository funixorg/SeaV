
#ifndef STDLIB_H
#define STDLIB_H

#include <cuteng/ce-libc/stdlib.h>
#include <cuteng/ce-libc/string.h>

#define MALLOC(size)

namespace std {
    void reverse(char str[], int length);
    void itoa(int num, char str[], int base);
}


#endif