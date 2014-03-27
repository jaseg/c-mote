
#include <i2c.h>

/* Note: We are not using ST's drivers here because they are kind of bloated. After all, we are getting along just find
 * with raw register accesses. */

void i2c_init(I2C_TypeDef *I2Cx){
	/* Force soft reset */
	I2Cx->CR1 = 0;
	I2Cx->CR1 = I2C_CR1_PE;
}

void i2c_disable(I2C_TypeDef *I2Cx){
	I2Cx->CR1 = 0;
}

int i2c_enabled(I2C_TypeDef *I2Cx){
	return !!(I2Cx->CR1&I2C_CR1_PE);
}

static int i2c_raw_write(I2C_TypeDef *I2Cx, uint8_t data){
	I2Cx->TXDR = data;
	while(!(I2Cx->ISR & I2C_FLAG_TXIS)); /* Wait for transfer */
	if(I2Cx->ISR & I2C_FLAG_NACKF) /* NACK received? */
		return -1;
	return 0;
}

static void i2c_raw_read(I2C_TypeDef *I2Cx, uint8_t *dst){
	while(!(I2Cx->ISR & I2C_FLAG_RXNE)); /* Wait for transfer */
	dst = I2Cx->RXDR;
}

int i2c_bulk_write(I2C_TypeDef *I2Cx, uint8_t device_addr, uint8_t reg, uint8_t *buf, size_t count){
	const uint8_t nbytes = 3;
    I2Cx->CR2 = I2C_CR2_AUTOEND | nbytes<<16 | I2C_CR2_START | device_addr;
	/* Now, the slave address is being transmitted. */
	while(!(I2Cx->ISR & I2C_FLAG_TXIS)); /* Wait for transfer */
	if(I2Cx->ISR & I2C_FLAG_NACKF) /* NACK received? */
		return -1;

	if(i2c_raw_write(ctx, reg))
		return -1;

	uint8_t *end = data+count;
	while(*data != end)
		if(i2c_raw_write(ctx, *data++))
			return -1;
	return 0;
}

int i2c_bulk_read(I2C_TypeDef *I2Cx, uint8_t device_addr, uint8_t reg, uint8_t *buf, size_t count){
	const uint8_t nbytes = 2;
    I2Cx->CR2 = I2C_CR2_AUTOEND | nbytes<<16 | I2C_CR2_START | device_addr;
	/* Now, the slave address is being transmitted. */
	while(!(I2Cx->ISR & I2C_FLAG_TXIS)); /* Wait for transfer */
	if(I2Cx->ISR & I2C_FLAG_NACKF) /* NACK received? */
		return -1;

	if(i2c_raw_write(ctx, reg))
		return -1;

    I2Cx->CR2 = I2C_CR2_AUTOEND | nbytes<<16 | I2C_CR2_WRN | I2C_CR2_START | device_addr;
	uint8_t *end = data+count;
	while(*data != end)
		i2c_raw_read(ctx, data++);

	return 0;
}

