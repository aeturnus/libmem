#ifndef __MEM_MEM_H__
#define __MEM_MEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

/*
 * Physical memory address access api
 * Provides a unix style read/write interface as well
 * as simpler put/get functions
 */

typedef enum _mem_mode
{
    MEM_FILE,   // internally treats this context like a normal file that has
                // to be seeked and read. useful if need to access many
                // different, non-contiguous registers
    MEM_MMAP    // internally mmap a range
} mem_mode;

typedef enum _mem_status
{
    MEM_OKAY,
    MEM_BAD_OPEN,
    MEM_BAD_MMAP
} mem_status;

typedef struct _mem_context
{
    int fd;     // memory device handle
    mem_mode mode;
    int write;  // write permission

    // mmap mode only data
    void * map; // mmap memory pointer
    void * s_page;  // real start passed to mmap() due to page boundary
    void * s_addr;  // start
    void * e_addr;  // end
    size_t map_range;   // range between s_addr and e_addr
    size_t map_len;     // needed for munmap()
} mem_context;

/**
 * Initializes a memory context. When mode is MEM_FILE, start_addr and
 * end_addr are not relevant.
 *
 * If mode is MEM_MMAP, the address range that is mapped will be
 * [start_addr, end_addr]
 * @param[in]   mode    The mode you want this context to operate in. If this is
 *                      MEM_FILE, the address parameters do not matter
 * @param[in]   write   If true, allows for writing
 * @param[in]   start_addr  If mode is MEM_MMAP, the start address of the physical
 *                          address space to map
 * @param[in]   end_addr    If mode is MEM_MMAP, the end address of the physical
 *                          address space to map
 * @return  Status of initialization
 */
mem_status mem_ctor(mem_context * mem, mem_mode mode, int write,
                    void * start_addr, void * end_addr);

/**
 * Destorys memory context
 */
void mem_dtor(mem_context * mem);

/**
 * Reads <count> bytes starting from <addr> into <buf>
 * @param[in]   addr
 */
ssize_t mem_read (mem_context * mem, void * addr, void * buf, size_t count);

/**
 * Writes <count> bytes from <buf> starting from <addr>
 * @param[in]   addr
 */
ssize_t mem_write(mem_context * mem, void * addr, const void * buf, size_t count);

#ifdef __cplusplus
}
#endif

#endif//__MEM_MEM_H__
