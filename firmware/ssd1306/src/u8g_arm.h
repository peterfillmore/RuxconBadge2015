#ifndef _U8G_ARM_H
#define _U8G_ARM_H
#include "u8g.h"
#include "i2c_func.h"
void delay_system_ticks(uint32_t sys_ticks);
void delay_micro_seconds(uint32_t us);

uint8_t u8g_com_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
#endif
