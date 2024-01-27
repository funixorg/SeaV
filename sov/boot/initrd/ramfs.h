#include <stdint.h>
#include <boot/initrd/ramdisk.h>

#ifndef RAMFS_H
#define RAMFS_H

struct File {
    char *name;
    char *path;
    void *address;
    uint64_t size;
};

struct Directory {
    char *name;
    char *path;
    File **files;
    Directory **directories;
    uint32_t dir_count;
    uint32_t file_count;
};


namespace RamFS {
    void initialize(initrd_header t_ramfs);
    void test();
}

#endif