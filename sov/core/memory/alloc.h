#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef MEM_H
#define MEM_H

void *malloc(size_t size);
void free(void *ptr);
void mempool_init(size_t heap_size);

#endif