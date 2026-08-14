#include "libunix.h"

void read_exact(const int fd, void *data, const uint32_t n)
{
    assert(n);
    int got;

    if ((got = read(fd, data, n)) < 0)
        sys_die(read, "read_exact failed!\n");
    if (got != n)
        panic("attempted to read %d bytes, only got %d\n", n, got);
}
