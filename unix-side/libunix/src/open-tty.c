// modified from engler, cs140e

#include <fcntl.h>
#include "libunix.h"

static int open_tty_n(const char *device, int maxattempts)
{
    int fd;
    for (uint32_t i = 0; i < maxattempts; i++)
    {
        // O_NOCTTY-- do not assign a controlling terminal. avoids the
        //      possibility of data from the pi being interpreted as syscall
        //      data, which could cause annoying, confusing bugs
        // O_SYNC-- write according to synchronized I/O file integrity
        //      completion; essentially, `write()`s blocks and returns only
        //      after bytes have been written
        if ((fd = open(device, O_RDWR | O_NOCTTY | O_SYNC)) >= 0)
        {
            output("opened tty port <%s>\n", device);
            return fd;
        }
        output("couldn't open tty port <%s>, going to sleep\n", device);
        sleep(1);
    }
    panic("couldn't open tty port <%s>!\n", device);
}

int open_tty(const char *device)
{
    // magic number, pay it no mind...
    const int maxattempts = 7;
    return open_tty_n(device, maxattempts);
}
