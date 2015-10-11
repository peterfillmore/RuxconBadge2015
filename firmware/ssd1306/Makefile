# put your *.o targets here, make should handle the rest!
SRCS = main.c system_stm32f0xx.c u8g_arm.c i2c_func.c

# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJ_NAME=main

# Location of the Libraries folder from the STM32F0xx Standard Peripheral Library
STD_PERIPH_LIB=Libraries
U8G_LIB=u8glib

# Location of the linker scripts
LDSCRIPT_INC=Device/ldscripts

#location of OpenOCD
OPENOCD_BASE_DIR=/usr/share/openocd
# location of OpenOCD Board .cfg files (only used with 'make program')
#OPENOCD_BOARD_DIR=$(OPENOCD_BASE_DIR)/scripts/board
OPENOCD_BOARD_DIR=./extra

# Configuration (cfg) file containing programming directives for OpenOCD
OPENOCD_PROC_FILE=extra/stm32f0-openocd.cfg

# that's it, no need to change anything below this line!

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
SIZE=arm-none-eabi-size

CFLAGS  = -Wall -g -std=c99 -Os  
#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m
CFLAGS += -mlittle-endian -mcpu=cortex-m0  -march=armv6-m -mthumb
CFLAGS += -ffunction-sections -fdata-sections -ffreestanding
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJ_NAME).map
#CFLAGS += -ggdb

###################################################

vpath %.c src
vpath %.a $(STD_PERIPH_LIB)
vpath %.a $(U8G_LIB)

ROOT=$(shell pwd)

CFLAGS += -I inc -I $(STD_PERIPH_LIB) -I $(STD_PERIPH_LIB)/CMSIS/Device/ST/STM32F0xx/Include
CFLAGS += -I $(STD_PERIPH_LIB)/CMSIS/Include -I $(STD_PERIPH_LIB)/STM32F0xx_StdPeriph_Driver/inc
CFLAGS += -include $(STD_PERIPH_LIB)/stm32f0xx_conf.h 
CFLAGS += -I$(U8G_LIB)/src -include $(U8G_LIB)/src/u8g.h

SRCS += Device/startup_stm32f0xx.s # add startup file to build

# need if you want to build with -DUSE_CMSIS 
#SRCS += stm32f0_discovery.c
#SRCS += stm32f0_discovery.c stm32f0xx_it.c

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj

lib:
	$(MAKE) -C $(STD_PERIPH_LIB)
	$(MAKE) -C $(U8G_LIB)

proj: 	$(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -L$(STD_PERIPH_LIB) -L$(U8G_LIB) -lstm32f0 -lu8g -L$(LDSCRIPT_INC) -Tstm32f0.ld
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -St $(PROJ_NAME).elf >$(PROJ_NAME).lst
	$(SIZE) $(PROJ_NAME).elf
	
program: $(PROJ_NAME).bin
	openocd -s $(OPENOCD_BASE_DIR)/scripts/ -f $(OPENOCD_BOARD_DIR)/stm32f030k6.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash `pwd`/$(PROJ_NAME).bin" -c shutdown

debug: $(PROJ_NAME).bin
	openocd -s $(OPENOCD_BASE_DIR)/scripts/ -f $(OPENOCD_BOARD_DIR)/stm32f030k6.cfg  


clean:
	find ./ -name '*~' | xargs rm -f	
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
	rm -f $(PROJ_NAME).lst

reallyclean: clean
	$(MAKE) -C $(STD_PERIPH_LIB) clean
	$(MAKE) -C $(U8G_LIB) clean
