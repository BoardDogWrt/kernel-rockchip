/* drivers/input/sensors/lsensor/bh1750.c
 *
 * Copyright (C) Guangzhou FriendlyARM Computer Tech. Co., Ltd.
 * (http://www.friendlyarm.com)
 *
 * Copyright (C) 2012-2015 ROCKCHIP.
 * Author: luowei <lw@rock-chips.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/of_gpio.h>
#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#include <linux/sensor-dev.h>

/* ------------------------------------------------------------- */
/* copy from drivers/iio/light/bh1750.c */

#define BH1750_POWER_DOWN		0x00
#define BH1750_ONE_TIME_H_RES_MODE	0x20 /* auto-mode for BH1721 */
#define BH1750_CHANGE_INT_TIME_H_BIT	0x40
#define BH1750_CHANGE_INT_TIME_L_BIT	0x60

struct bh1750_chip_info;

struct bh1750_data {
	struct mutex lock;
	struct i2c_client *client;
	const struct bh1750_chip_info *chip_info;
	u16 mtreg;
};

struct bh1750_chip_info {
	u16 mtreg_min;
	u16 mtreg_max;
	u16 mtreg_default;
	int mtreg_to_usec;
	int mtreg_to_scale;

	/*
	 * For BH1710/BH1721 all possible integration time values won't fit
	 * into one page so displaying is limited to every second one.
	 * Note, that user can still write proper values which were not
	 * listed.
	 */
	int inc;

	u16 int_time_low_mask;
	u16 int_time_high_mask;
}

static const bh1750_chip_info = {
	31,  254, 69,  1740, 57500000,  1, 0x001F, 0x00E0
};

static int bh1750_change_int_time(struct bh1750_data *data, int usec)
{
	int ret;
	u16 val;
	u8 regval;
	const struct bh1750_chip_info *chip_info = data->chip_info;

	if ((usec % chip_info->mtreg_to_usec) != 0)
		return -EINVAL;

	val = usec / chip_info->mtreg_to_usec;
	if (val < chip_info->mtreg_min || val > chip_info->mtreg_max)
		return -EINVAL;

	ret = i2c_smbus_write_byte(data->client, BH1750_POWER_DOWN);
	if (ret < 0)
		return ret;

	regval = (val & chip_info->int_time_high_mask) >> 5;
	ret = i2c_smbus_write_byte(data->client,
				   BH1750_CHANGE_INT_TIME_H_BIT | regval);
	if (ret < 0)
		return ret;

	regval = val & chip_info->int_time_low_mask;
	ret = i2c_smbus_write_byte(data->client,
				   BH1750_CHANGE_INT_TIME_L_BIT | regval);
	if (ret < 0)
		return ret;

	data->mtreg = val;

	return 0;
}

static int bh1750_read(struct bh1750_data *data, int *val)
{
	int ret;
	__be16 result;
	const struct bh1750_chip_info *chip_info = data->chip_info;
	unsigned long delay = chip_info->mtreg_to_usec * data->mtreg;

	/*
	 * BH1721 will enter continuous mode on receiving this command.
	 * Note, that this eliminates need for bh1750_resume().
	 */
	ret = i2c_smbus_write_byte(data->client, BH1750_ONE_TIME_H_RES_MODE);
	if (ret < 0)
		return ret;

	usleep_range(delay + 15000, delay + 40000);

	ret = i2c_master_recv(data->client, (char *)&result, 2);
	if (ret < 0)
		return ret;

	*val = be16_to_cpu(result);

	return 0;
}

/* ------------------------------------------------------------- */

static int sensor_active(struct i2c_client *client, int enable, int rate)
{
	return 0;
}

static int sensor_init(struct i2c_client *client)
{
	struct sensor_private_data *sensor =
	    (struct sensor_private_data *) i2c_get_clientdata(client);
	struct bh1750_data *data;
	int ret, usec;

	data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->client = client;
	data->chip_info = &bh1750_chip_info;

	/* H-reslution mode: lx/count = 1 / 1.2 * (69 / 57) = 1.008772 */
	usec = data->chip_info->mtreg_to_usec * 57;

	ret = bh1750_change_int_time(data, usec);
	if (ret < 0)
		return ret;

	mutex_init(&data->lock);

	sensor->driver_data = data;
	sensor->status_cur = SENSOR_OFF;
	return 0;
}

static int sensor_report_value(struct i2c_client *client)
{
	struct sensor_private_data *sensor =
	    (struct sensor_private_data *) i2c_get_clientdata(client);
	struct bh1750_data *data = (struct bh1750_data *) sensor->driver_data;
	int ret, raw = -1;

	mutex_lock(&data->lock);
	ret = bh1750_read(data, &raw);
	mutex_unlock(&data->lock);

	if (ret < 0)
		return ret;

	if (raw > 0) {
		/* magic for sensor-HAL to skip indexToValue */
		raw += 8;

		input_report_abs(sensor->input_dev, ABS_MISC, raw);
		input_sync(sensor->input_dev);
	}

	return 0;
}

struct sensor_operate light_bh1750_ops = {
	.name				= "bh1750",
	.type				= SENSOR_TYPE_LIGHT,
	.id_i2c				= LIGHT_ID_BH1750,
	.read_reg			= SENSOR_UNKNOW_DATA,
	.read_len			= 2,
	.id_reg				= SENSOR_UNKNOW_DATA,
	.id_data 			= SENSOR_UNKNOW_DATA,
	.precision			= 16,
	.ctrl_reg 			= SENSOR_UNKNOW_DATA,
	.int_status_reg 		= SENSOR_UNKNOW_DATA,
	.range				= {8,65535},
	.brightness			= {1,255},
	.trig				= SENSOR_UNKNOW_DATA,
	.active				= sensor_active,
	.init				= sensor_init,
	.report				= sensor_report_value,
};

/* ------------------------------------------------------------- */

static int light_bh1750_probe(struct i2c_client *client,
			      const struct i2c_device_id *devid)
{
	return sensor_register_device(client, NULL, devid, &light_bh1750_ops);
}

static int light_bh1750_remove(struct i2c_client *client)
{
	return sensor_unregister_device(client, NULL, &light_bh1750_ops);
}

static const struct i2c_device_id light_bh1750_id[] = {
	{"light_bh1750", LIGHT_ID_AL3006},
	{}
};

static struct i2c_driver light_bh1750_driver = {
	.probe = light_bh1750_probe,
	.remove = light_bh1750_remove,
	.shutdown = sensor_shutdown,
	.id_table = light_bh1750_id,
	.driver = {
		.name = "light_bh1750",
#ifdef CONFIG_PM
		.pm = &sensor_pm_ops,
#endif
	},
};

module_i2c_driver(light_bh1750_driver);

MODULE_AUTHOR("support@friendlyarm.com");
MODULE_DESCRIPTION("bh1750 light driver");
MODULE_LICENSE("GPL");

