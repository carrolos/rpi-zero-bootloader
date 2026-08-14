# About
NOTE: still in development; not yet fully-functional

This repo contains a custom bootloader for the rpi zero (and zero w). With this
bootloader, you can send programs to the pi over a uart connection rather than
needing to manually
- change the name of the program to `kernel.img`
- remove SD card from pi
- attach SD card to development machine
- replace `kernel.img` on the pi with the newly-created `kernel.img`
- remove SD card from dev machine
- attach SD card to pi
**every single time** you want to run a program on the pi.

To accomplish this, there's a 'unix' side and a 'pi' side of the bootloader,
which are both contained in this repo. The pi side makes use of a private
library, which is explained in more detail below.

Note: you must still do the above manual steps to install the bootloader on the
pi, but the point of the bootloader is that it becomes the 'permanent'
`kernel.img`.

# Getting Started
If you actually want to run the programs on the pi, you'll need:
- a pi zero
- a way to power the pi
- an sd card
- a USB to TTL serial adapter
- compiler toolchain: arm-none-eabi (available on Linux, Windows, and macOS)

# Install
This repo makes use of a git submodule which contains the necessary bare-metal
code (linker script, gpio driver, etc.,) for the program(s) in this repo to
work. Since the library is inspired by Stanford's CS140E, it's private to
prevent the distribution of solutions to current/future students.

**Recruiters/employers**: if you'd like access to the library, please email me
at browses.epitaph.0e@icloud.com and I'll be happy to provide access.

Clone the repo and fetch the submodule
```
git clone --recurse-submodules git@github.com:carrolos/simple-blinky.git
```

If you stopped reading after 'Clone' and need to fetch the submodule:
```
git submodule update --init
```

# Running the program
## pi side
To build the programs on the pi, run
```
make all
```
and then run
```
cp pi-bootload.c kernel.img
```

Then copy `firmware/*` and `kernel.img` into your sd card.

## unix side
To build the unix side, run
```
make all
```
you should now have a program called `unix-side/objs/pi-install.out` and another
program called `unix-side/objs/pi-char-echo.out`.

## using the bootloader
You now can bootload a program onto the pi! To bootload a program, say,
`unix-side/objs/pi-char-echo.out`:
- ensure SD card is attached to the pi
- ensure pi is powered and connected to your dev machine you'll run the
    bootloader on

and then run
```
unix-side/objs/pi-install.out unix-side/objs/pi-char-echo.out
```
.

You should see some stuff printed onto the screen, and then a message that
invites you to type a character. When you do so, the character will travel
from your input device -> your device's stdin -> pi via tty device
-> your dev machine via tty device -> your stdout!
