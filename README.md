# Reader firmware

Firmware for the [Reader](https://github.com/fmfi-svt-gate/server/wiki/Architecture#reader) component of project Gate. For the project overview, and more information about what the reader does and what communication protocols it utilizes see https://github.com/fmfi-svt-gate/server/wiki.

## Compiling

The reader utilizes the ATTiny841 MCU, which is not supported by the current versions of avr-gcc (4.9.2) and avr-libc (1.8.1). Therefore this has to be compiled with patched versions of these programs. The patches can be found here: https://github.com/fmfi-svt-gate/avr-toolchain-patches.

After installing the patched versions compiling is as easy as running `make`.

## Flashing the firmware

After compiling the firmware you can flash it with your favorite tool.

We are using the opensource tool [AVRDUDE](http://www.nongnu.org/avrdude/). `make` targets for this tool are prepared in the Makefile for convenience. The default configuration presumes using an AVR ISP MkII programmer connected to a USB port, but you can use any programmer which is supported by AVRDUDE, just edit the variables `AVRDUDE_PRG` and `AVRDUDE_PRG_PORT`.

But it would be too easy if AVRDUDE supported ATTiny841, would't it? ;). The configuration file adding support for this MCU can be found here: https://github.com/fmfi-svt-gate/avr-toolchain-patches/tree/master/avrdude-config

The prepared targets are:

- `chip_erase`
- `chip_write_flash`
- `chip_write_eeprom`
- `chip_write` (writes both flash and eeprom)
- `open_terminal` (opens the AVRDUDE interactive terminal)
