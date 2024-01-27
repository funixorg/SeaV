#include <stddef.h>
#include <stdint.h>
#include <std/stdio.h>
#include <std/stdlib.h>

#ifndef TAR_H
#define TAR_H

#define TAR_DIR '5'
#define TAR_FILE '0'

struct __attribute__((packed)) htars90_dir {
    uint32_t entry_size;
    uint16_t path_size;
    uint16_t name_size;
    char *path;
    char *name;
    uint16_t n_file;
    uint16_t n_dir;
    uint32_t hfile_size;
    uint32_t hdir_size;
    uint16_t cont_off;
};

struct __attribute__((packed)) htars90_dir_fixed {
    uint16_t n_file;
    uint16_t n_dir;
    uint32_t hfile_size;
    uint32_t hdir_size;
    uint16_t cont_off;
};

namespace TAR {
    htars90_dir read_header(void *address);
}


#endif