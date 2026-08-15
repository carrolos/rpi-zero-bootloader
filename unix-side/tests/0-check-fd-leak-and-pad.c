// test ripped from engler, cs140e, with minor edits
#include <string.h>

#include "libunix.h"

// see which fd the system returns
static int fd_next(void)
{
    const char *filename = "temp1.txt";
    int fd = create_file(filename);
    // unlinking first guarantees the resources are cleaned up by the kernel,
    // even if our process dies before closing the file
    // TODO: uncomment below after seeing what gets written when don't use &
    // and when use & below in main; must manually delete temp files, however
    unlink(filename);
    close(fd);
    return fd;
}

int main(void)
{
    const char* name = "temp0.txt";    
    const char* hello = "hello";

    const int fd = create_file(name);
    uint32_t i;
    for (i = 0; hello[i]; i++)
        // note that we give the address of a byte! cuz write_exact expects
        // a void*
        write_exact(fd, &hello[i], 1);
    // write the terminator
    write_exact(fd, &hello[i], 1);
    close(fd);

    // should get same fd each time
    for (i = 0; i < 3; i++)
    {
        if (fd_next() != fd_next())
            panic("not getting same fd when we should!\n");
    }

    // ensure read_file doesn't lose the fd
    int fd_before = fd_next();

    // read file should open and close fd, so should get same below
    uint32_t nbytes;
    char *data = read_file(&nbytes, name);
    unlink(name);

    int fd_after = fd_next();
    if (fd_before != fd_after)
        panic("it seems that read_file leaks file descriptors!\n");

    uint32_t n = strlen(hello) + 1;
    if (nbytes != n)
        panic("expected %d bytes, only have %d\n", n, nbytes);
    output("success! expected %d bytes, got %d\n", n, nbytes);

    if (strcmp(hello, data) != 0)
        panic("expected string <%s>, have <%s>\n", hello, data);
    output("success! expected string <%s>, have <%s>\n", hello, data);

    // check padded bytes
    uint32_t round = pi_roundup(nbytes, 4);
    for (i = nbytes; i < round; i++)
        assert(data[i] == 0);
    free(data);
    output("success!\n");
    return 0;
}
