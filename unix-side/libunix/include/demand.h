// ripped from CS140E, with some edits and omissions
// NOTE: all of these automatically stream output to stderr; if you need
// something else, edit these or add new ones

#ifndef __DEMAND_H__
#define __DEMAND_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// turn the macro argument into a string literal
#define STRINGIFY(x) #x

// output to stderr without attaching the file/line
#define output(msg...) fprintf(stderr, ##msg)

#define notreached()    panic("hit NOTREACHED!\n")
#define unimplemented() panic("unimplemented code\n")

#define AssertNow(x) switch(1) { case(x): case 0: ; }

#define trace(msg...) do { output("TRACE:"); output(msg); } while(0)

// if you pass -DNDEBUG to the compiler, these are optimized away
#ifndef NDEBUG
#   define demand(_expr, _msg, args...) do { } while(0)
#   define debug(msg...) do { } while(0)
#else

#   define demand(_expr, _msg, args...) do {                         \
        if (!(_expr)) {                                              \
            fprintf(stderr, "%s:%s:%d: Assertion '%s' failed:",      \
                __FILE__, __FUNCTION__, __LINE__, STRINGIFY(_expr)); \
            fprintf(stderr, STRINGIFY(_msg) "\n", ##args);           \
            exit(1);                                                 \
        }                                                            \
} while(0)
#   define debug(msg...) do {                                           \
        fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(stderr, ##msg);                                         \
    } while(0)
#endif

// various fatal error macros

// called when <syscall> fails, print <msg> and the Unix perror value, and die
// note that when -DNDEBUG is passed, debug no longer exists, but the rest of
// the routine still exists
#define sys_die(syscall, msg, args...) do {                                 \
        debug("FATAL syscall error: " STRINGIFY(msg) "\n\tperror reason: ", \
            ##args);                                                        \
        perror(STRINGIFY(syscall));                                         \
        exit(1);                                                            \
    } while(0)

#define no_fail(syscall) do {                                               \
        if((syscall) < 0) {                                                 \
            int reason = errno;                                             \
            sys_die(syscall,                                                \
                "<%s> failed: errno=%d: we thought this'd be impossible\n", \
                STRINGIFY(syscall), reason);                                \
        }                                                                   \
    } while(0)

#define clean_exit(msg...) do { \
        fprintf(stderr, ##msg); \
        exit(0);                \
    } while(0)

// die without prepending information
#define die(msg...) do {        \
        fprintf(stderr, ##msg); \
        exit(1);                \
    } while (0)

// die with a panic message
#define panic(msg...) do {                                          \
        output("%s:%s:%d:PANIC", __FILE__, __FUNCTION__, __LINE__); \
        die(msg);                                                   \
    } while(0)

#define todo(msg) panic("TODO: %s\n", msg)

#endif
