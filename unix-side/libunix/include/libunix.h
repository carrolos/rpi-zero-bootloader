#ifndef __LIBUNIX_H__
#define __LIBUNIX_H__

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

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


#include "demand.h"

#endif
