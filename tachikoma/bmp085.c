/*  Copyright (c) 2010  Christoph Mair <christoph.mair@gmail.com>
 *  Copyright (c) 2012  Bosch Sensortec GmbH
 *  Copyright (c) 2012  Unixphere AB
 *  Adapted for c-mote/tachikoma by Sebastian Götte
 *
 *  This driver supports the bmp085 and bmp18x digital barometric pressure
 *  and temperature sensors from Bosch Sensortec. The datasheets
 *  are available from their website:
 *  http://www.bosch-sensortec.com/content/language1/downloads/BST-BMP085-DS000-05.pdf
 *  http://www.bosch-sensortec.com/content/language1/downloads/BST-BMP180-DS000-07.pdf
 *
 *  A pressure measurement is issued by reading from pressure0_input.
 *  The return value ranges from 30000 to 110000 pascal with a resulution
 *  of 1 pascal (0.01 millibar) which enables measurements from 9000m above
 *  to 500m below sea level.
 *
 *  The temperature can be read from temp0_input. Values range from
 *  -400 to 850 representing the ambient temperature in degree celsius
 *  multiplied by 10.The resolution is 0.1 celsius.
 *
 *  Because ambient pressure is temperature dependent, a temperature
 *  measurement will be executed automatically even if the user is reading
 *  from pressure0_input. This happens if the last temperature measurement
 *  has been executed more then one second ago.
 *
 *  To decrease RMS noise from pressure measurements, the bmp085 can
 *  autonomously calculate the average of up to eight samples. This is
 *  set up by writing to the oversampling sysfs file. Accepted values
 *  are 0, 1, 2 and 3. 2^x when x is the value written to this file
 *  specifies the number of samples used to calculate the ambient pressure.
 *  RMS noise is specified with six pascal (without averaging) and decreases
 *  down to 3 pascal when using an oversampling setting of 3.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "bmp085.h"
#include "config.h"
#include <stint.h>
#include <errno.h>

#define u8  uint8_t
#define s8  int8_t
#define u16	uint16_t
#define s16 int16_t
#define u32 uint32_t
#define s32 int32_t

#define BMP085_CHIP_ID			0x55
#define BMP085_CALIBRATION_DATA_START	0xAA
#define BMP085_CALIBRATION_DATA_LENGTH	11	/* 16 bit values */
#define BMP085_CHIP_ID_REG		0xD0
#define BMP085_CTRL_REG			0xF4
#define BMP085_TEMP_MEASUREMENT		0x2E
#define BMP085_PRESSURE_MEASUREMENT	0x34
#define BMP085_CONVERSION_REGISTER_MSB	0xF6
#define BMP085_CONVERSION_REGISTER_LSB	0xF7
#define BMP085_CONVERSION_REGISTER_XLSB	0xF8
#define BMP085_TEMP_CONVERSION_TIME	5

struct bmp085_calibration_data {
	s16 AC1, AC2, AC3;
	u16 AC4, AC5, AC6;
	s16 B1, B2;
	s16 MB, MC, MD;
};

struct bmp085_data {
	struct	bmp085_calibration_data calibration;
	s32	b6; /* calculated temperature correction coefficient */
};

static s32 bmp085_read_calibration_data(struct bmp085_data *data)
{
	u16 tmp[BMP085_CALIBRATION_DATA_LENGTH];
	struct bmp085_calibration_data *cali = &(data->calibration);
	s32 status = i2c_bulk_read(BMP085_I2C, BMP085_CALIBRATION_DATA_START, (u8 *)tmp, BMP085_CALIBRATION_DATA_LENGTH*sizeof(u16));
	if (status < 0)
		return status;

	cali->AC1 = be16toh(tmp[0]);
	cali->AC2 = be16toh(tmp[1]);
	cali->AC3 = be16toh(tmp[2]);
	cali->AC4 = be16toh(tmp[3]);
	cali->AC5 = be16toh(tmp[4]);
	cali->AC6 = be16toh(tmp[5]);
	cali->B1  = be16toh(tmp[6]);
	cali->B2  = be16toh(tmp[7]);
	cali->MB  = be16toh(tmp[8]);
	cali->MC  = be16toh(tmp[9]);
	cali->MD  = be16toh(tmp[10]);
	return 0;
}

static s32 bmp085_update_raw_temperature(struct bmp085_data *data)
{
	u16 tmp;
	s32 status;

	status = i2c_bulk_write(BMP085_I2C, BMP085_CTRL_REG, &BMP085_TEMP_MEASUREMENT, 1);
	if(status < 0)
		goto error;
	/* FIXME: Add timeout of BMP085_TEMP_CONVERSION_TIME milliseconds */
	while(!BMP085_GPIO->IDR & (1<<BMP085_PIN));

	status = i2c_bulk_read(BMP085_I2C, BMP085_CONVERSION_REGISTER_MSB, &tmp, sizeof(tmp));
	if(status < 0)
		goto error;

	return betoh16(tmp);
error:
	return status;
}

static s32 bmp085_update_raw_pressure(struct bmp085_data *data)
{
	u32 tmp = 0;
	s32 status;

	status = i2c_bulk_write(BMP085_I2C, BMP085_CTRL_REG, BMP085_PRESSURE_MEASUREMENT+(BMP085_OVERSAMPLING<<6), 1);
	if(status < 0)
		goto error;

	/* FIXME add timeout of (2+(3 << BMP085_OVERSAMPLING)) milliseconds */
	while(!BMP085_GPIO->IDR & (1<<BMP085_PIN));

	/* copy data into a u32 (4 bytes), but skip the first byte. */
	status = i2c_bulk_read(BMP085_I2C, BMP085_CONVERSION_REGISTER_MSB, ((u8 *)&tmp)+1, 3);
	if(status < 0)
		goto error;

	return betoh32(tmp) >> (8-BMP085_OVERSAMPLING);;
error:
	return status;
}

/*
 * This function starts the temperature measurement and returns the value
 * in tenth of a degree celsius.
 */
static s32 bmp085_get_temperature(struct bmp085_data *data, int *temperature)
{
	struct bmp085_calibration_data *cali = &data->calibration;
	long x1, x2;
	int status;

	status = bmp085_update_raw_temperature(data);
	if (status < 0)
		goto exit;

	x1 = ((status - cali->AC6) * cali->AC5) >> 15;
	x2 = (cali->MC << 11) / (x1 + cali->MD);
	data->b6 = x1 + x2 - 4000;
	/* if NULL just update b6. Used for pressure only measurements */
	if (temperature != NULL)
		*temperature = (x1+x2+8) >> 4;

exit:
	return status;
}

/*
 * This function starts the pressure measurement and returns the value
 * in millibar. Since the pressure depends on the ambient temperature,
 * a temperature measurement is executed according to the given temperature
 * measurement period (default is 1 sec boundary). This period could vary
 * and needs to be adjusted according to the sensor environment, i.e. if big
 * temperature variations then the temperature needs to be read out often.
 * FIXME: There is a PDF floating around that does the computations in this function at much greater precision using
 * floating-point calculations. We should really use this here since we have an FPU.
 */
// FIXME: Convert this to use floating point math for slightly better precision
static s32 bmp085_get_pressure(struct bmp085_data *data, int *pressure)
{
	struct bmp085_calibration_data *cali = &data->calibration;
	s32 x1, x2, x3, b3;
	u32 b4, b7;
	s32 p;
	int status;

	/* at least every second force an update of the ambient temperature to update data->b6 */
	/* FIXME */
	if ((data->last_temp_measurement == 0) ||
	    time_is_before_jiffies(data->last_temp_measurement + 1*HZ)) {
		status = bmp085_get_temperature(data, NULL);
		if (status < 0)
			return status;
	}

	status = bmp085_update_raw_pressure(data);
	if (status < 0)
		return status;

	x1 = (data->b6 * data->b6) >> 12;
	x1 *= cali->B2;
	x1 >>= 11;

	x2 = cali->AC2 * data->b6;
	x2 >>= 11;

	x3 = x1 + x2;

	b3 = (((((s32)cali->AC1) * 4 + x3) << data->oversampling_setting) + 2);
	b3 >>= 2;

	x1 = (cali->AC3 * data->b6) >> 13;
	x2 = (cali->B1 * ((data->b6 * data->b6) >> 12)) >> 16;
	x3 = (x1 + x2 + 2) >> 2;
	b4 = (cali->AC4 * (u32)(x3 + 32768)) >> 15;

	b7 = ((u32)status - b3) *
					(50000 >> data->oversampling_setting);
	p = ((b7 < 0x80000000) ? ((b7 << 1) / b4) : ((b7 / b4) * 2));

	x1 = p >> 8;
	x1 *= x1;
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	p += (x1 + x2 + 3791) >> 4;

	*pressure = p;

	return 0;
}

int bmp085_detect(){
	unsigned int id;
	int err = -ENODEV;

	int i2c_en = i2c_enabled(BMP085_I2C);
	i2c_init(BMP085_I2C);

	err = i2c_bulk_read(BMP085_I2C, BMP085_CHIP_ID_REG, &id, 1);
	if(err < 0)
		goto cleanup;

	if(id != BMP085_CHIP_ID)
		goto cleanup;

	return 0;
cleanup:
	if(!i2c_en)
		i2c_disable(BMP085_I2C);
	return err;
}

int bmp085_init(struct bmp085_data *data){
	int err;

	err = bmp085_detect();
	if(err < 0)
		return err;
		
	err = bmp085_read_calibration_data(data);
	if(err < 0)
		return err;

	return 0;
}

