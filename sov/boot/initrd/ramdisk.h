#include <stdint.h>

#ifndef RAMDISK_H
#define RAMDISK_H

struct initrd_header {
    uint32_t size;
    void *address;
};


namespace Ramdisk {
    void initialize();
}

#endif