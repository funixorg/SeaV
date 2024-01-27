#include <boot/initrd/ramfs.h>
#include <boot/initrd/ramdisk.h>
#include <boot/initrd/tar.h>
#include <std/stdlib.h>
#include <stddef.h>

struct off_dc_fc {
    uint64_t offset;
    unsigned dir_index;
    unsigned file_index;
};

initrd_header a_ramfs;

namespace RamFS {
    void initialize(initrd_header t_ramfs) {
        a_ramfs = t_ramfs;
    }

    Directory *tar_to_struct() {
        htars90_dir tsdh = TAR::read_header(a_ramfs.address);
        std::SerialIO::printf("NAME: %s\nHSIZE: %d\nNFILE: %d\nNDIR: %d\n", tsdh.path, tsdh.entry_size, tsdh.n_file, tsdh.n_dir);
        return NULL;
    }

    void test() {
        Directory *root = tar_to_struct();
    }
}