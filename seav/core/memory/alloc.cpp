#include <stddef.h>
#include <stdint.h>
#include <core/memory/alloc.h>

static char *memory_pool = nullptr;
static size_t pool_size = 0;

struct Block {
    size_t size;
    bool free;
};

void *malloc(size_t size) {
    if (memory_pool == nullptr) {
        return nullptr;
    }

    Block *current_block = reinterpret_cast<Block*>(memory_pool);
    while (current_block->size < size || !current_block->free) {
        uintptr_t block_address = reinterpret_cast<uintptr_t>(current_block);
        block_address += sizeof(Block) + current_block->size;
        if (block_address >= reinterpret_cast<uintptr_t>(memory_pool) + pool_size) {
            // No suitable block found
            return nullptr;
        }
        current_block = reinterpret_cast<Block*>(block_address);
    }

    if (current_block->size > size + sizeof(Block)) {
        Block *next_block = reinterpret_cast<Block*>(reinterpret_cast<uintptr_t>(current_block) + sizeof(Block) + size);
        next_block->size = current_block->size - size - sizeof(Block);
        next_block->free = true;
        current_block->size = size;
    }

    current_block->free = false;
    return reinterpret_cast<void*>(current_block + 1);
}

void free(void *ptr) {
    if (memory_pool == nullptr || ptr == nullptr) {
        return;
    }

    Block *current_block = (Block*)(ptr) - 1;
    current_block->free = true;
}

void mempool_init(size_t heap_size) {
    pool_size = heap_size;
    char pool[heap_size];
    memory_pool = pool;
    Block *initial_block = reinterpret_cast<Block*>(memory_pool);
    initial_block->size = pool_size - sizeof(Block);
    initial_block->free = true;
}
