#include <boot/bootapi/api.h>
#include <std/stdlib.h>
#include <std/stdio.h>
#include <core/serial/serial.h>
#include <limine.h>
#include <cuteng/ce-libc/stdlib.h>

#include <stddef.h>

LIMINE_BASE_REVISION(1);

struct BootFields {
    limine_framebuffer *framebuffer;
    limine_memmap_entry **memmap_entries;
};

BootFields boot_values;

namespace BootAPI {
    void _framebuffer_load() {
        struct limine_framebuffer_response framebuffer_response;
        struct limine_framebuffer_request framebuffer_request = {
            .id = LIMINE_FRAMEBUFFER_REQUEST,
            .revision = 0,
            .response = &framebuffer_response
        };
        if (LIMINE_BASE_REVISION_SUPPORTED == false) {
            return;
        }
        if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
            return;
        }
        boot_values.framebuffer = framebuffer_request.response->framebuffers[0];
    }

    void _mem_load() {
        struct limine_memmap_response memmap_response;
        struct limine_memmap_request memmap_request {
            .id = LIMINE_MEMMAP_REQUEST,
            .revision = 0,
            .response = &memmap_response
        };
        boot_values.memmap_entries = memmap_request.response->entries;
        size_t free_mem = memmap_request.response->entries[LIMINE_MEMMAP_USABLE]->length;
        //Mem::initialize_memory_pool(free_mem);
    }

    void feat_load() {
        Serial::initialize();
        _mem_load();
        _framebuffer_load();

        std::SerialIO::printf("HERE\n");
    }

    limine_framebuffer *get_fb() {
        return boot_values.framebuffer;
    }
}