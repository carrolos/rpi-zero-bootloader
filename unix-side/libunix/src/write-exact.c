// ripped from engler, cs140e

#include "libunix.h"

// dies if unsuccessful
void write_exact(const int fd, const void *data, const uint32_t n)
{
    assert(n);

    int got;
    if ((got = write(fd, data, n)) < 0)
        sys_die(write, "write exact failed\n");
    if (got != n)
        panic("expected to write %d bytes, only wrote %d\n", n, got);
}
