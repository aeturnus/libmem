#include <stdio.h>
#include <stdint.h>

#include <mem/mem.h>

//#define MMAP

int main(int argc, char ** argv)
{
    printf("mem utility testing version\n");

    if (argc < 2) {
        printf("Please provide an address: mem <address>\n");
        return -1;
    }

    void * read_addr = NULL;

    sscanf(argv[1], "%p", &read_addr);

    if (read_addr == NULL)
        return -1;


    mem_context mem;
    #ifndef MMAP
    printf("non-mmap version\n");
    if (mem_ctor(&mem, MEM_FILE, 0, NULL, NULL) != MEM_OKAY) {
    #else
    printf("mmap version\n");
    if (mem_ctor(&mem, MEM_MMAP, 0, (void *) read_addr,
                (void *) ((uintptr_t) read_addr + sizeof(uint32_t) - 1))
        != MEM_OKAY) {
    #endif
        printf("Failed to start up libmem context\n");
        return -1;
    }

    uint32_t val = 0xDEADBEEF;
    mem_read(&mem, read_addr, &val, sizeof(val));
    printf("[%p] = 0x%08x\n", read_addr, val);

    mem_dtor(&mem);

    return 0;
}
