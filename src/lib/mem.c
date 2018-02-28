#include <string.h>

#include <unistd.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mem/mem.h>

mem_status mem_ctor(mem_context * mem, mem_mode mode, int write,
                    void * start_addr, void * end_addr)
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC, S_IRUSR | (write ? S_IWUSR: 0));
    if (fd == -1) {
        return MEM_BAD_OPEN;
    }

    if (mode == MEM_MMAP) {
        uintptr_t start = (uintptr_t) start_addr;
        uintptr_t end   = (uintptr_t) end_addr;
        size_t pg_size = sysconf(_SC_PAGE_SIZE);
        size_t pg_mask = pg_size - 1;   // mask covering the addresses in a page


        size_t offset = start & ~pg_mask;   // mmap offset is on page boundary
        size_t remain = start - offset;     // space before start that starts from
                                            // page boundary
        size_t range = end - start + 1;
        size_t length = remain + range;

        mem->map = mmap(NULL, length, PROT_READ | (write ? PROT_WRITE: 0),
                        MAP_SHARED, fd, offset);

        if (mem->map == MAP_FAILED) {
            close(fd);
            return MEM_BAD_MMAP;
        }
        mem->map_len = length;
        mem->s_page = (void *) offset;
        mem->s_addr = start_addr;
        mem->e_addr = end_addr;
        mem->range  = end - start + 1;

    } else {
        mem->s_addr = NULL;
        mem->e_addr = NULL;
    }

    mem->fd    = fd;
    mem->mode  = mode;
    mem->write = write;

    return MEM_OKAY;
}

void mem_dtor(mem_context * mem)
{
    if (mem->mode == MEM_MMAP) {
        munmap(mem->map, mem->map_len);
    }
    close(mem->fd);
}

// calculate the offset from the map base address
// since mem->s_page was mapped to the mem->map, get the offset from
// mem->s_page and add it to mem->map
static inline
void * calc_map_addr(mem_context * mem, void * addr)
{
    return (void *) ((uintptr_t) addr - (uintptr_t) mem->s_page +
                     (uintptr_t) mem->map);
}

// common transfer function
#define D_RD 0
#define D_WR 1
static
ssize_t mem_xfer (mem_context * mem, void * addr, void * buf,
                  size_t count, int dir)
{
    ssize_t xfer_bytes = -1;
    if (mem->mode == MEM_MMAP) {
        if (addr < mem->s_addr || mem->e_addr < addr)
            return xfer_bytes;

        void * map_addr = calc_map_addr(mem, addr);

        if (dir == D_RD) {
            memcpy(buf, map_addr, count);
        } else {
            memcpy(map_addr, buf, count);
        }
        xfer_bytes = count;
    } else {
        // move the read head to correct offset and read
        off_t offset = (off_t) (uintptr_t) addr;
        off_t pos = lseek(mem->fd, offset, SEEK_SET);
        if (pos < 0) {
            return xfer_bytes;
        }
        if (dir == D_RD) {
            xfer_bytes = read(mem->fd, buf, count);
        } else {
            xfer_bytes = write(mem->fd, buf, count);
        }
    }
    return xfer_bytes;
}

ssize_t mem_read(mem_context * mem, void * addr, void * buf, size_t count)
{
    return mem_xfer(mem, addr, buf, count, D_RD);
}

ssize_t mem_write(mem_context * mem, void * addr, const void * buf, size_t count)
{
    return mem_xfer(mem, addr, (void *) buf, count, D_WR);
}

void * mem_addr(mem_context * mem, void * addr)
{
    if (mem->mode != MEM_MMAP)
        return NULL;

    return calc_map_addr(mem, addr);
}

void * mem_addr_off(mem_context * mem, uintptr_t offset)
{
    if (mem->mode != MEM_MMAP)
        return NULL;

    if (offset > (uintptr_t) mem->range)
        return NULL;

    void * addr = mem_addr(mem, (void *) ((uintptr_t) mem->s_addr + offset));

    return addr;
}
