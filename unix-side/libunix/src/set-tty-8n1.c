// inspired by engler, cs140e, with modifications

#include "libunix.h"

// sets the tty port to 8n1 mode with Baudrate <speed>; times out after
// <timeout> seconds (note that units is seconds and type is float, so
// <timeout> < 1 is OK)
// panics if can't set the port to desired settings
void set_tty_to_8n1(int fd, uint32_t speed, double timeout)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    // we clear anyway but check for possible failures:
    // EBADF-- fd is not a valid file descriptor (should be impossible)
    // ENOTTY-- file associated with fd is not a terminal
    if (tcgetattr(fd, &tty) != 0)
        panic("tcgetattr failed; errno=<%d>\n", errno);
    memset(&tty, 0, sizeof tty);

    cfsetspeed(&tty, speed);

    assert(timeout < 100 && timeout > 0);
    //  VTIME is in .1 seconds, so have to multiply by 10
    tty.c_cc[VTIME] = (int)(timeout * 10);
    
    // set data bits = 8
    tty.c_cflag |= CS8;
    // enable receiver, ignore modem control lines
    tty.c_cflag |= CREAD | CLOCAL;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
        panic("tcsetattr failed; errno=<%d\n", errno);
}


