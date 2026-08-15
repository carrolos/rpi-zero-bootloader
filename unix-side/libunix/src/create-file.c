// ripped from engler, cs140e, with minor edits
#include <fcntl.h>

#include "libunix.h"

// creates the file, if possible, and returns the file descriptor for it
// dies if unsuccessful
int create_file(const char *name)
{
    int fd = open(name, O_RDWR | O_CREAT | O_TRUNC | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1)
        sys_die(open, "could not open file: <%s>\n", name);
    return fd;
}
