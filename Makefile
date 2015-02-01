MCU    = attiny841

# MCU frequency in Hz
F_CPU  = 1000000

AVRDUDE_MCU       = t841
AVRDUDE_PRG       = avrispmkii
AVRDUDE_PRG_PORT  = usb

# Speed of communication between programmer and MCU, see avrdude(1) for explanation
AVRDUDE_BITCLOCK  = 10

CC = avr-gcc

CFLAGS = -W -Wall -Wextra -pedantic -std=c11 -Os

# `char` should be `unsigned char`, and bitfields should have unsigned type as well.
CFLAGS += -funsigned-char -funsigned-bitfields

# Do not leave holes in structures, this may save us some RAM. Docs mention that this generates
# suboptimal code: this is true only on platforms which need to have aligned access to the RAM.
# This is not the case for AVR architecture.
CFLAGS += -fpack-struct

# Enums should have size of their largest element.
CFLAGS += -fshort-enums

# Together with -Wl,-gc-sections this will remove unused functions and data sections.
CFLAGS += -fdata-sections -ffunction-sections

# Prepare the binary files for link-time optimizations. This allows all the interprocedural
# analyses and optimizations in GCC to work across more files as if they were a single one.
CFLAGS += -flto

# Optimize nested loops
CFLAGS += -fno-tree-scev-cprop

CFLAGS += -mmcu=$(MCU) -DF_CPU=$(F_CPU)

LDFLAGS = -lm -Wl,-gc-sections -Wl,--relax 

SRCS = $(shell find -name '*.c')
OBJS = $(SRCS:.c=.o)

default: main.hex main.eep

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

%.eep: %.elf
	avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex $< $@

main.elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o main.elf
	avr-size --format=avr --mcu=$(MCU) main.elf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d

.PHONY: chip_erase chip_write_flash chip_write_eeprom chip_write open_terminal print_size clean

chip_erase:
	avrdude -p $(AVRDUDE_MCU) -c $(AVRDUDE_PRG) -P $(AVRDUDE_PRG_PORT) -B $(AVRDUDE_BITCLOCK) -e

chip_write_flash: main.hex
	avrdude -p $(AVRDUDE_MCU) -c $(AVRDUDE_PRG) -P $(AVRDUDE_PRG_PORT) -B $(AVRDUDE_BITCLOCK) -U flash:w:main.hex:a

# In the following recipe avrdude would segfault if it attempts to write empty eeprom. This is a workaround
chip_write_eeprom: main.eep
	@if avr-size -A main.eep | grep "^Total\s*0" -q; \
	then \
		echo "EEPROM empty, nothing to write"; \
	else \
		avrdude -p $(AVRDUDE_MCU) -c $(AVRDUDE_PRG) -P $(AVRDUDE_PRG_PORT) -B $(AVRDUDE_BITCLOCK) -U eeprom:w:main.eep; \
	fi

chip_write: chip_write_flash chip_write_eeprom
    
open_terminal:
	avrdude -p $(AVRDUDE_MCU) -c $(AVRDUDE_PRG) -P $(AVRDUDE_PRG_PORT) -t

print_size: main.elf
	avr-nm -S --size-sort -r main.elf
	avr-size --format=avr --mcu=$(MCU) main.elf

clean:
	find . -name '*.[od]' -exec rm -f '{}' \;
	rm -f *.elf

# include the generated dependencies
-include $(OBJS:.o=.d)
