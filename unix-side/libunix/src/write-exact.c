// ripped from engler, cs140e

#include "libunix.h"

// returns number of bytes written; panics if failed or wrote less than n
int write_exact(int fd, const void *data, uint32_t n)
{
    assert(n);

    int got;
    if ((got = write(fd, data, n)) < 0)
        sys_die(write, "write exact failed\n");
    if (got != n)
        panic("expected to write %d bytes, only wrote %d\n", n, got);
    return n;
}
