#include <fcntl.h>
#include <sys/stat.h>

#include "libunix.h"

// allocate buffer, read entire file into it, return it.   
// buffer is zero padded to a multiple of 4.
//
//  - <size> = *exact* nbytes of file (not padded size)
//  - for allocation: round up allocated size to 4-byte multiple, pad
//    buffer with 0s. 
//
// fatal error: open/read of <name> fails.
//   - make sure to check all system calls for errors.
//   - make sure to close the file descriptor (this will
//     matter for later labs).
// 
void *read_file(uint32_t *size, const char *name) {
    // use stat() to get the size of the file.
    struct stat statbuf;
    if ((stat(name, &statbuf)) == -1)
        sys_die(stat, "stat failed on file %s!\n", name);

    // round up to a multiple of 4.
    const uint32_t n = pi_roundup(statbuf.st_size, 4);

    // open the file
    const int fd = open(name, O_RDONLY);
    if (fd == -1)
        sys_die(open, "open() failed in read_file for file=<%s>\n", name);

    // allocate a buffer
    char *ret = malloc(n);

    // read entire file into buffer
    read_exact(fd, ret, statbuf.st_size);

    // close() the file descriptor
    if (close(fd) == -1)
        sys_die(close, "close() failed in read_file for file=<%s>\n", name);

    // zero pads to a multiple of 4.
    for (uint32_t i = statbuf.st_size; i < n; i++)
        ret[i] = 0;

    // deliverables
    *size = statbuf.st_size;
    return ret;
}

