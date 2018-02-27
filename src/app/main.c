#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <mem/mem.h>

#define MMAP

int main(int argc, char ** argv)
{
    printf("mem utility testing version\n");

    if (argc < 2) {
        printf("Please provide an address: mem <address> [value to write]\n");
        return -1;
    }

    int write = 0;
    if (argc >= 3) {
        write = 1;
    }

    void * read_addr = NULL;

    sscanf(argv[1], "%p", &read_addr);

    if (read_addr == NULL)
        return -1;

    mem_context mem;
    #ifndef MMAP
    printf("non-mmap version\n");
    if (mem_ctor(&mem, MEM_FILE, write, NULL, NULL) != MEM_OKAY) {
    #else
    printf("mmap version\n");
    if (mem_ctor(&mem, MEM_MMAP, write, (void *) read_addr,
                (void *) ((uintptr_t) read_addr + sizeof(uint32_t) - 1))
        != MEM_OKAY) {
    #endif
        printf("Failed to start up libmem context\n");
        return -1;
    }

    int32_t val = 0xDEADBEEF;
    int bytes_xfer = -1;
    if (write) {
        // write
        size_t len = strlen(argv[2]);
        if (len > 2 && argv[2][0] == '0' && argv[2][1] == 'x') {
            sscanf(argv[2], "0x%x", &val);
        } else {
            sscanf(argv[2], "%d", &val);
        }
        bytes_xfer = mem_write(&mem, read_addr, &val, sizeof(val));
        if (bytes_xfer < 0) {
            printf("Write failed (returned -1)\n");
        } else {
            printf("[%p] = 0x%08x\n", read_addr, val);
        }
    } else {
        bytes_xfer = mem_read(&mem, read_addr, &val, sizeof(val));
        if (bytes_xfer < 0) {
            printf("Read failed (returned -1)\n");
        } else {
            printf("[%p] = 0x%08x\n", read_addr, val);
        }
    }

    mem_dtor(&mem);

    return bytes_xfer >= 0 ? 0 : -1;
}
