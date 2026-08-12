#ifndef __LIBUNIX_H__
#define __LIBUNIX_H__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
// used by open-tty.c
#include <unistd.h>
// used by rpz-install.c and set-tty-8n1.c
#include <termios.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// return 1 if suffix/prefix are in s
int suffix_cmp(const char *s, const char *suffix);
int prefix_cmp(const char *s, const char *prefix);

// looks in /dev for a ttyusb device
// returns: the device name
char *find_ttyusb_first(void);
// will be the default used in bootloader
char *find_ttyusb_last(void);

// attempts to open the device some number of times
// panics if is unable to do so
int open_tty(const char *device);

// sets a tty port to the 8n1 protocol
// panics if is unable to do so
void set_tty_to_8n1(int fd, uint32_t speed, double timeout);

#include "demand.h"

#endif
