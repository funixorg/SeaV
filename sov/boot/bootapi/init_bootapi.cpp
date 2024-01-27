#include <boot/bootapi/api.h>
#include <std/stdlib.h>
#include <std/stdio.h>

#include <core/serial/serial.h>
#include <core/memory/gdt.h>
#include <core/memory/alloc.h>

#include <boot/initrd/ramdisk.h>

#include <limine.h>
#include <cuteng/ce-libc/stdlib.h>

#include <stddef.h>

LIMINE_BASE_REVISION(1);

struct BootFields {
    limine_framebuffer *framebuffer;
    limine_memmap_entry **memmap_entries;
    limine_smp_response *smp_info;
    limine_paging_mode_response *paging_info;
    uint64_t stack_size;
    uint64_t heap_size;
    uint64_t heap_addr;
};



BootFields boot_values;

struct limine_memmap_response memmap_response;
struct limine_paging_mode_response paging_response;
struct limine_framebuffer_response framebuffer_response;
struct limine_stack_size_response stacksize_response;
struct limine_smp_response smp_response;

volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = LIMINE_BASE_REVISION_SUPPORTED,
    .response = &framebuffer_response
};
volatile struct limine_paging_mode_request paging_request = {
    .id = LIMINE_PAGING_MODE_REQUEST,
    .revision = LIMINE_BASE_REVISION_SUPPORTED,
    .response = &paging_response,
    .mode = LIMINE_PAGING_MODE_X86_64_4LVL,
    .flags = 0,
};

volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = LIMINE_BASE_REVISION_SUPPORTED,
    .response = &memmap_response
};

volatile struct limine_stack_size_request stacksize_request {
    .id = LIMINE_STACK_SIZE_REQUEST,
    .revision = LIMINE_BASE_REVISION_SUPPORTED,
    .response = &stacksize_response
};

struct limine_smp_request smp_request {
    .id = LIMINE_SMP_REQUEST,
    .revision = LIMINE_BASE_REVISION_SUPPORTED,
    .response = &smp_response,
    .flags = 0
};


namespace BootAPI {
    void _framebuffer_load() {
        if (LIMINE_BASE_REVISION_SUPPORTED == false) { return; }
        if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) { return; }
        boot_values.framebuffer = framebuffer_request.response->framebuffers[0];
    }

    void _mem_load() {
        std::SerialIO::printf("[*] Requesting Paging...\n");
        boot_values.paging_info = paging_request.response;
        std::SerialIO::printf("[*] Memory Mapping...\n");
        boot_values.memmap_entries = memmap_request.response->entries;
        std::SerialIO::printf("[*] Requesting stack size...\n");
        boot_values.stack_size = stacksize_request.stack_size;
        for (unsigned _i=0; _i<memmap_request.response->entry_count; _i++) {
            if (boot_values.memmap_entries[_i]->type == LIMINE_MEMMAP_USABLE) {
                if (!boot_values.heap_addr) {
                    boot_values.heap_addr+=boot_values.memmap_entries[_i]->base;
                }
                boot_values.heap_size+=boot_values.memmap_entries[_i]->length;
            }
        }
    }

    void _cpu_load() {
        std::SerialIO::printf("[*] Preparing SMP...\n");
        boot_values.smp_info = smp_request.response;
    }

    void _ramdisk_load() {
        std::SerialIO::printf("[*] Loading ramdisk into memory...\n");
        Ramdisk::initialize();
    }

    void feat_load() {
        Serial::initialize();

        _mem_load();
        _framebuffer_load();
        _cpu_load();
        gdt_init();

        mempool_init(boot_values.heap_size);

        _ramdisk_load();
    }

    limine_framebuffer *get_fb() {
        return boot_values.framebuffer;
    }
}