#ifndef I2CFUNC_H
#define I2CFUNC_H
#include "stm32f0xx_conf.h"

I2C_InitTypeDef I2C1_InitStruct;

void init_i2c(void);
void I2C_WrReg(uint8_t Reg, uint8_t Val);
void i2c_out(uint8_t val);
#endif
