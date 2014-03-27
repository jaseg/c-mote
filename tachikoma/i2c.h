#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

void i2c_reg_bulk_read(I2C_TypeDef* ctx, uint8_t device_addr, uint8_t reg, uint8_t *dst, size_t count);

#endif//__I2C_H__
