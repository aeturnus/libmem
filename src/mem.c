#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mem/mem.h>

void mem_ctor(mem_context * mem, mem_mode mode, void * start_addr, void * end_addr)
{
}

void mem_dtor(mem_context * mem)
{
}

ssize_t mem_read (mem_context * mem, void * addr, void * buf, size_t count)
{
    return -1;
}

ssize_t mem_write(mem_context * mem, void * addr, const void * buf, size_t count)
{
    return -1;
}
