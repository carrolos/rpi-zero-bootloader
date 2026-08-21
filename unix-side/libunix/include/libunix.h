#ifndef __LIBUNIX_H__
#define __LIBUNIX_H__

#include <assert.h>
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
// roundup <x> to a multiple of <n>: taken from the lcc compiler, ripped
// from engler, cs140e
#define pi_roundup(x,n) (((x)+((n)-1))&(~((n)-1)))

// return 1 if suffix/prefix are in s
int suffix_cmp(const char *s, const char *suffix);
int prefix_cmp(const char *s, const char *prefix);

// looks in /dev for a ttyusb device
// returns: the device name
char *find_ttyusb_first(void);
// will be the default used in bootloader
char *find_ttyusb_last(void);
// finds the ttyusb device specified by <path>; returns if it exists, and
// dies internally otherwise
void find_ttyusb(const char *path);

// attempts to open the device some number of times
// panics if is unable to do so
int open_tty(const char *device);

// sets a tty port to the 8n1 protocol
// panics if is unable to do so
void set_tty_to_8n1(const int fd, const uint32_t speed, const double timeout);

// writes exactly n bytes; dies if `write()` itself fails and panics if writes
// less than n bytes
void write_exact(const int fd, const void *data, const uint32_t n);
// reads exactly n bytes; dies if `read()` itself fails and panics if reads
// less than n bytes
void read_exact(const int fd, void *data, const uint32_t n);

// reads file <name> into memory
// returns:
//  - pointer to the code; code is padded with 0s up to next multiple of 4
// mutates:
//  - number of bytes in <size>
//
// panics if can't read file
void *read_file(uint32_t *size, const char *name);

// creates the file and returns the file descriptor
// dies if `open()` fails
int create_file(const char *name);

#include "demand.h"

#endif
