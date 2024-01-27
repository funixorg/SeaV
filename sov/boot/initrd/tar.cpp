#include <boot/initrd/tar.h>
#include <std/stdlib.h>
#include <std/stdio.h>

namespace TAR {
    htars90_dir read_header(void *address) {
        uint32_t offset=0;
        uint32_t entry_size = *(uint32_t*)(address+offset); offset+=sizeof(uint32_t);
        uint16_t path_size  = *(uint16_t*)(address+offset); offset+=2;
        uint16_t name_size  = *(uint16_t*)(address+offset); offset+=2;
        char *path = (char*)malloc(sizeof(char)*path_size);
        char *name = (char*)malloc(sizeof(char)*name_size);
        memcpy(path, (address+offset), path_size); offset +=path_size;
        memcpy(name, (address+offset), name_size); offset +=name_size;
        htars90_dir_fixed *fixed_header = (htars90_dir_fixed*)(address+offset);
        htars90_dir dir_header = {
            .entry_size = entry_size,
            .path_size = path_size,
            .name_size = name_size,
            .path = path,
            .name = name,
            .n_file = fixed_header->n_file,
            .n_dir = fixed_header->n_dir,
            .hfile_size = fixed_header->hfile_size,
            .hdir_size = fixed_header->hdir_size,
            .cont_off = fixed_header->cont_off
        };
        return dir_header;
    }
}