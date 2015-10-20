CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c src

#CFLAGS  = -g -O2 -Wall
CFLAGS  = -g -Os -Wall
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m
CFLAGS += -ffreestanding -nostdlib -Isrc -includesrc/u8g.h
#CFLAGS += -includestm32f0xx_conf.h -ICMSIS/Include -ICMSIS/Device/ST/STM32F0xx/Include -ISTM32F0xx_StdPeriph_Driver/inc

SRCS = u8g_pb8h1f.c u8g_bitmap.c u8g_pb8h2.c u8g_circle.c u8g_pb8h8.c u8g_clip.c u8g_pb8v1.c \
u8g_com_api_16gr.c u8g_pb8v2.c u8g_com_api.c u8g_dev_null.c u8g_pb.c u8g_com_i2c.c u8g_ellipse.c u8g_pbxh16.c \
u8g_com_io.c u8g_font.c u8g_pbxh24.c u8g_com_null.c u8g_font_data.c u8g_polygon.c \
u8g_line.c u8g_rect.c u8g_dev_ssd1306_128x64.c u8g_ll_api.c \
u8g_rot.c u8g_cursor.c u8g_page.c u8g_scale.c u8g_delay.c u8g_pb14v1.c u8g_state.c \
u8g_pb16h1.c u8g_u16toa.c u8g_pb16h2.c u8g_u8toa.c u8g_pb16v1.c u8g_virtual_screen.c u8g_pb16v2.c u8g_pb32h1.c u8g_pb8h1.c

OBJS = $(SRCS:.c=.o)

.PHONY: libu8g.a

all: libu8g.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libu8g.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f $(OBJS) libu8g.a 
