// simple program where rpi echos input a little fancier than that found in
// test-uart/, cuz of surrounding infrastructure that replaces the need to
// use the `screen` utility

#include <stdio.h>
#include <signal.h>

#include "libunix.h"
#include "echo-defs.h"

#define RESTORE_RETRIES 3

static struct termios original;

void cleanup(void)
{
    int retries = RESTORE_RETRIES;
    while (retries-- > 0)
    {
        if (tcsetattr(STDIN_FILENO, TCSANOW, &original) == 0)
            break;
    }
}

void restore_terminal(int sig)
{
    cleanup();
    signal(sig, SIG_DFL);
    raise(sig);
}

void put_uint8(int fd, uint8_t b) { write_exact(fd, &b, 1); }

uint8_t get_uint8(int fd)
{
    uint8_t b;

    int res;
    if ((res = read(fd, &b, 1)) < 0)
    {
        cleanup();
        die("rpz-echo: tty-usb read() returned error = %d; disconnected?\n",
            res);
    }
    if (res == 0)
    {
        cleanup();
        die("rpz-echo: tty-usb read() returned 0 bytes; pi not responding\n");
    }
    assert(res == 1);
    return b;
}

int main(void)
{
    // get original terminal settings
    memset(&original, 0, sizeof original);
    if (tcgetattr(STDIN_FILENO, &original) != 0)
        panic("tcgetattr failed on stdin!\n");
     
    // find the device
    char* dev_name;
    for (uint32_t i = 0; i < MAX_RETRIES; i++)
    {
        dev_name = find_ttyusb_last();
        if (dev_name[0] != '\0')
            break;
        printf("did not find a device; sleeping for a second\n");
        sleep(1);
    }

    if (dev_name[0] == '\0')
    {
        die("rpz-echo: couldn't find a device after %d tries; dying\n",
            MAX_RETRIES);
    }

    // setup the device
    debug("found device <%s>; will now attempt to connect\n", dev_name);
    const int tty = open_tty(dev_name);
    debug("connected! setting up baudrate, etc., on tty device\n");
    const double timeout_tenths = 2 * 8;
    // Bx...s defined in termios.h, included in libunix.h
    const uint32_t baud_rate = B115200;
    set_tty_to_8n1(tty, baud_rate, timeout_tenths);
    debug("tty set up; setting up terminal to communicate with pi\n"); 

    // tell the kernel that we'll handle these signals
    signal(SIGINT, restore_terminal);
    signal(SIGTERM, restore_terminal);

    atexit(cleanup);

    // set stdin to raw mode
    struct termios modified = original;
    modified.c_lflag &= ~(ICANON);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &modified) != 0)
        // notice that we haven't changed anything, so no teardown
        panic("failed to change stdin settings!\n");

    debug("set up complete. ready for a char to send to pi: \n");
    uint8_t c;
    for (uint32_t i = 0; i < MAX_RETRIES; i++)
    {
        read(STDIN_FILENO, &c, 1);
        put_uint8(tty, c);
        c = get_uint8(tty);
//        putchar(c);
//        putc(c, stderr);
        put_uint8(STDOUT_FILENO, c);

//        putchar('\n');
//        uint8_t c_tx;
//        uint8_t c_rx;
//        while ((c_tx = getc(STDIN_FILENO)) != SPECIAL_CHAR)
//            put_uint8(tty, c_tx);
//        // actually put the special char
//        put_uint8(tty, c_tx);
//        while ((c_rx = get_uint8(tty)) != SPECIAL_CHAR)
//            putchar(c_rx);
//        putchar('\n');
    }

    debug("program done; cleaning up\n");

    if (close(tty) == -1)
        die("couldn't properly close tty!\n");

    return 0;
}
