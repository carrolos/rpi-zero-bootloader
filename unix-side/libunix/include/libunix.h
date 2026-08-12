#ifndef __LIBUNIX_H__
#define __LIBUNIX_H__

#include <string.h>
#include <stdbool.h>

int suffix_cmp(const char *s, const char *suffix);
int prefix_cmp(const char *s, const char *prefix);

#include "demand.h"

#endif
