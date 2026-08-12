// simple program where rpi echos input a little fancier than that found in
// test-uart/, cuz of surrounding infrastructure that replaces the need to
// use the `screen` utility

#include <stdio.h>
#include "libunix.h"

void put_uint8(int fd, uint8_t b) { write_exact(fd, &b, 1); }

uint8_t get_uint8(int fd)
{
    uint8_t b;

    int res;
    if ((res = read(fd, &b, 1)) < 0)
    {
        die("rpz-echo: tty-usb read() returned error = %d; disconnected?\n",
            res);
    }
    if (res == 0)
        die("rpz-echo: tty-usb read() returned 0 bytes; pi not responding\n");
    assert(res == 1);
    return b;
}

int main(void)
{
    char* dev_name;
    uint32_t max_tries = 7;
    for (uint32_t i = 0; i < max_tries; i++)
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
            max_tries);
    }

    printf("found device <%s>; will now attempt to connect\n", dev_name);
    int tty = open_tty(dev_name);
    printf("connected! setting up baudrate, etc...\n");
    double timeout_tenths = 2 * 5;
    // Bx...s defined in termios.h, included in libunix.h
    uint32_t baud_rate = B115200;
    set_tty_to_8n1(tty, baud_rate, timeout_tenths);

    printf("baudrate set up; will now communicate with pi!\n\n"); 
    for (uint32_t i = 0; i < max_tries; i++)
    {
        uint8_t c_tx;
        uint8_t c_rx;
        while ((c_tx = getc(STDIN_FILENO)) != '!')
            put_uint8(tty, c_tx);
        while ((c_rx = get_uint8(tty)) != '!')
            putchar(c_rx);
    }
    int e = close(tty);
    if (e == -1)
        sys_die(close, "close failed!\n");
    return 0;
}
