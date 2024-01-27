#include <boot/initrd/ramdisk.h>
#include <boot/initrd/tar.h>
#include <boot/initrd/ramfs.h>
#include <limine.h>

#include <stddef.h>
#include <std/stdlib.h>
#include <std/stdio.h>

struct limine_module_response mods_response;
struct limine_module_request mods_request {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0,
    .response = &mods_response,
};

initrd_header c_ramfs;

namespace Ramdisk {
    void _read_ramdisk(void *address, uint32_t size) {
        c_ramfs.size    = size;
        c_ramfs.address = address;
    }

    void _full_parse() {
        RamFS::initialize(c_ramfs);
        RamFS::test();
    }

    void initialize() {
        if (mods_request.response->module_count < 1) {
            return;
        }
        struct limine_file *ramdisk_file = mods_request.response->modules[0];
        Ramdisk::_read_ramdisk(ramdisk_file->address, ramdisk_file->size);
        _full_parse();
    }
}