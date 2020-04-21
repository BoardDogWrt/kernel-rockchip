// SPDX-License-Identifier: GPL-2.0
/*
 * dw9718 vcm driver
 *
 * Copyright (c) 2020 FriendlyElec Computer Tech. Co., Ltd.
 * (http://www.friendlyarm.com)
 *
 * Copyright (C) 2019 Fuzhou Rockchip Electronics Co., Ltd.
 */

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/rk-camera-module.h>
#include <linux/version.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include "rk_vcm_head.h"

#define DRIVER_VERSION			KERNEL_VERSION(0, 0x01, 0x0)
#define DW9718_NAME			"dw9718"

#define DW9718_MAX_CURRENT		100U
#define DW9718_MAX_REG			1023U

/* Register addresses */
#define DW9718_PD			0x00
#define DW9718_CONTROL			0x01
#define DW9718_DATA_M			0x02
#define DW9718_DATA_L			0x03
#define DW9718_SACT			0x05

#define DW9718_CONTROL_SW_LINEAR	BIT(0)
#define DW9718_CONTROL_S_SAC4		(BIT(1) | BIT(3))
#define DW9718_CONTROL_OCP_DISABLE	BIT(4)
#define DW9718_CONTROL_UVLO_DISABLE	BIT(5)

#define DW9718_SACT_MULT_TWO		0x00
#define DW9718_SACT_PERIOD_8_8MS	0x19
#define DW9718_SACT_DEFAULT_VAL		0x60

#define DW9718_DEFAULT_START_CURRENT	0
#define DW9718_DEFAULT_RATED_CURRENT	100
#define DW9718_DEFAULT_STEP_MODE	0xd
#define REG_NULL			0xFF

/* dw9718 device structure */
struct dw9718_device {
	struct v4l2_ctrl_handler ctrls_vcm;
	struct v4l2_subdev sd;

	unsigned short current_related_pos;
	unsigned short current_lens_pos;
	unsigned int start_current;
	unsigned int rated_current;
	unsigned int step;
	unsigned int step_mode;
	unsigned int vcm_movefull_t;
	unsigned int dlc_enable;
	unsigned int t_src;
	unsigned int mclk;

	struct timeval start_move_tv;
	struct timeval end_move_tv;
	unsigned long move_ms;

	u32 module_index;
	const char *module_facing;
};

struct TimeTabel_s {
	unsigned int t_src;/* time of slew rate control */
	unsigned int step00;/* S[1:0] /MCLK[1:0] step period */
	unsigned int step01;
	unsigned int step10;
	unsigned int step11;
};

static const struct TimeTabel_s dw9718_lsc_time_table[] = {
	{0b10000, 136, 272, 544, 1088},
	{0b10001, 130, 260, 520, 1040},
	{0b10010, 125, 250, 500, 1000},
	{0b10011, 120, 240, 480, 960 },
	{0b10100, 116, 232, 464, 928 },
	{0b10101, 112, 224, 448, 896 },
	{0b10110, 108, 216, 432, 864 },
	{0b10111, 104, 208, 416, 832 },
	{0b11000, 101, 202, 404, 808 },
	{0b11001,  98, 196, 392, 784 },
	{0b11010,  95, 190, 380, 760 },
	{0b11011,  92, 184, 368, 736 },
	{0b11100,  89, 178, 356, 712 },
	{0b11101,  87, 174, 348, 696 },
	{0b11110,  85, 170, 340, 680 },
	{0b11111,  83, 166, 332, 664 },
	{0b00000,  81, 162, 324, 648 },
	{0b00001,  79, 158, 316, 632 },
	{0b00010,  77, 155, 310, 620 },
	{0b00011,  76, 152, 304, 608 },
	{0b00100,  74, 149, 298, 596 },
	{0b00101,  73, 146, 292, 584 },
	{0b00110,  71, 143, 286, 572 },
	{0b00111,  70, 140, 280, 560 },
	{0b01000,  69, 138, 276, 552 },
	{0b01001,  68, 136, 272, 544 },
	{0b01010,  67, 134, 268, 536 },
	{0b01011,  66, 132, 264, 528 },
	{0b01100,  65, 131, 262, 524 },
	{0b01101,  65, 130, 260, 520 },
	{0b01110,  64, 129, 258, 516 },
	{0b01111,  64, 128, 256, 512 },
	{REG_NULL,  0, 0, 0, 0},
};

static const struct TimeTabel_s dw9718_dlc_time_table[] = {/* us */
	{0b10000, 21250, 10630, 5310, 2660},
	{0b10001, 20310, 10160, 5080, 2540},
	{0b10010, 19530,  9770, 4880, 2440},
	{0b10011, 18750,  9380, 4690, 2340},
	{0b10100, 18130,  9060, 4530, 2270},
	{0b10101, 17500,  8750, 4380, 2190},
	{0b10110, 16880,  8440, 4220, 2110},
	{0b10111, 16250,  8130, 4060, 2030},
	{0b11000, 15780,  7890, 3950, 1970},
	{0b11001, 15310,  7660, 3830, 1910},
	{0b11010, 14840,  7420, 3710, 1860},
	{0b11011, 14380,  7190, 3590, 1800},
	{0b11100, 13910,  6950, 3480, 1740},
	{0b11101, 13590,  6800, 3400, 1700},
	{0b11110, 13280,  6640, 3320, 1660},
	{0b11111, 12970,  6480, 3240, 1620},
	{0b00000, 12660,  6330, 3160, 1580},
	{0b00001, 12340,  6170, 3090, 1540},
	{0b00010, 12110,  6050, 3030, 1510},
	{0b00011, 11880,  5940, 2970, 1480},
	{0b00100, 11640,  5820, 2910, 1460},
	{0b00101, 11410,  5700, 2850, 1430},
	{0b00110, 11170,  5590, 2790, 1400},
	{0b00111, 10940,  5470, 2730, 1370},
	{0b01000, 10780,  5390, 2700, 1350},
	{0b01001, 10630,  5310, 2660, 1330},
	{0b01010, 10470,  5230, 2620, 1310},
	{0b01011, 10310,  5160, 2580, 1290},
	{0b01100, 10230,  5120, 2560, 1280},
	{0b01101, 10160,  5080, 2540, 1270},
	{0b01110, 10080,  5040, 2520, 1260},
	{0b01111, 10000,  5000, 2500, 1250},
	{REG_NULL, 0, 0, 0, 0},
};

static inline struct dw9718_device *to_dw9718_vcm(struct v4l2_ctrl *ctrl)
{
	return container_of(ctrl->handler, struct dw9718_device, ctrls_vcm);
}

static inline struct dw9718_device *sd_to_dw9718_vcm(struct v4l2_subdev *subdev)
{
	return container_of(subdev, struct dw9718_device, sd);
}

static int dw9718_i2c_rd8(struct i2c_client *client, u8 reg, u8 *val)
{
	struct i2c_msg msg[2];
	u8 buf[2] = { reg };

	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = buf;

	msg[0].addr = client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = 1;
	msg[1].buf = &buf[1];
	*val = 0;

	if (i2c_transfer(client->adapter, msg, 2) != 2)
		return -EIO;
	*val = buf[1];

	return 0;
}

static int dw9718_i2c_wr8(struct i2c_client *client, u8 reg, u8 val)
{
	struct i2c_msg msg;
	u8 buf[2] = { reg, val};

	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = sizeof(buf);
	msg.buf = buf;

	if (i2c_transfer(client->adapter, &msg, 1) != 1)
		return -EIO;

	return 0;
}

static int dw9718_i2c_wr16(struct i2c_client *client, u8 reg, u16 val)
{
	struct i2c_msg msg;
	u8 buf[3] = { reg, (u8)(val >> 8), (u8)(val & 0xff)};

	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = sizeof(buf);
	msg.buf = buf;

	if (i2c_transfer(client->adapter, &msg, 1) != 1)
		return -EIO;

	return 0;
}

static unsigned int dw9718_move_time(struct dw9718_device *priv,
		unsigned int move_pos)
{
	struct i2c_client *client = v4l2_get_subdevdata(&priv->sd);
	unsigned int move_time_ms = 200;
	unsigned int step_period_lsc = 0;
	unsigned int step_period_dlc = 0;
	unsigned int step_period = 0;
	unsigned int codes_per_step = 1;
	int table_cnt = 0;
	int i = 0;
	int step_case;

	if (priv->dlc_enable) {
		step_case = priv->mclk & 0x3;
		table_cnt = sizeof(dw9718_dlc_time_table) /
					sizeof(struct TimeTabel_s);
		for (i = 0; i < table_cnt; i++) {
			if (dw9718_dlc_time_table[i].t_src == priv->t_src)
				break;
		}
	} else {
		step_case = priv->step_mode & 0x3;
		table_cnt = sizeof(dw9718_lsc_time_table) /
					sizeof(struct TimeTabel_s);
		for (i = 0; i < table_cnt; i++) {
			if (dw9718_lsc_time_table[i].t_src == priv->t_src)
				break;
		}
	}

	if (i >= table_cnt)
		i = 0;

	switch (step_case) {
	case 0:
		step_period_lsc = dw9718_lsc_time_table[i].step00;
		step_period_dlc = dw9718_dlc_time_table[i].step00;
		break;
	case 1:
		step_period_lsc = dw9718_lsc_time_table[i].step01;
		step_period_dlc = dw9718_dlc_time_table[i].step01;
		break;
	case 2:
		step_period_lsc = dw9718_lsc_time_table[i].step10;
		step_period_dlc = dw9718_dlc_time_table[i].step10;
		break;
	case 3:
		step_period_lsc = dw9718_lsc_time_table[i].step11;
		step_period_dlc = dw9718_dlc_time_table[i].step11;
		break;
	default:
		dev_err(&client->dev,
			"%s: step_case is error %d\n", __func__, step_case);
		break;
	}

	codes_per_step = (priv->step_mode & 0x0c) >> 2;
	if (codes_per_step > 1)
		codes_per_step = 1 << (codes_per_step - 1);

	if (priv->dlc_enable)
		step_period = step_period_dlc;
	else
		step_period = step_period_lsc;

	if (!codes_per_step)
		move_time_ms = step_period * move_pos / 1000;
	else
		move_time_ms = step_period * move_pos / codes_per_step / 1000;

	return move_time_ms;
}

static int dw9718_get_pos(struct dw9718_device *priv,
		unsigned int *cur_pos)
{
	struct i2c_client *client = v4l2_get_subdevdata(&priv->sd);

	*cur_pos = priv->current_related_pos;
	dev_dbg(&client->dev, "%s: get position %d\n", __func__, *cur_pos);

	return 0;
}

static int dw9718_set_pos(struct dw9718_device *priv,
		unsigned int dest_pos)
{
	struct i2c_client *client = v4l2_get_subdevdata(&priv->sd);
	u16 position;
	int ret;

	if (dest_pos >= VCMDRV_MAX_LOG)
		position = priv->start_current;
	else
		position = priv->start_current +
			(priv->step * (VCMDRV_MAX_LOG - dest_pos));

	if (position > DW9718_MAX_REG)
		position = DW9718_MAX_REG;

	ret = dw9718_i2c_wr16(client, DW9718_DATA_M, position);
	if (ret < 0) {
		dev_err(&client->dev, "%s: failed with error %d\n", __func__, ret);
		return ret;
	}

	priv->current_lens_pos = position;
	priv->current_related_pos = dest_pos;

	return 0;
}

static int dw9718_get_ctrl(struct v4l2_ctrl *ctrl)
{
	struct dw9718_device *priv = to_dw9718_vcm(ctrl);

	if (ctrl->id == V4L2_CID_FOCUS_ABSOLUTE)
		return dw9718_get_pos(priv, &ctrl->val);

	return -EINVAL;
}

static int dw9718_set_ctrl(struct v4l2_ctrl *ctrl)
{
	struct dw9718_device *priv = to_dw9718_vcm(ctrl);
	struct i2c_client *client = v4l2_get_subdevdata(&priv->sd);
	unsigned int dest_pos = ctrl->val;
	int move_pos;
	long int mv_us;
	int ret = 0;

	if (ctrl->id == V4L2_CID_FOCUS_ABSOLUTE) {
		if (dest_pos > VCMDRV_MAX_LOG) {
			dev_err(&client->dev,
				"%s: invalid dest_pos, %d > %d\n",
				__func__, dest_pos, VCMDRV_MAX_LOG);
			return -EINVAL;
		}

		/* calculate move time */
		move_pos = priv->current_related_pos - dest_pos;
		if (move_pos < 0)
			move_pos = -move_pos;

		ret = dw9718_set_pos(priv, dest_pos);

		priv->move_ms =
			((priv->vcm_movefull_t * (uint32_t)move_pos) /
			 VCMDRV_MAX_LOG);
		dev_dbg(&client->dev,
			"dest_pos %d, move_ms %ld\n", dest_pos, priv->move_ms);

		priv->start_move_tv = ns_to_timeval(ktime_get_ns());
		mv_us = priv->start_move_tv.tv_usec + priv->move_ms * 1000;
		if (mv_us >= 1000000) {
			priv->end_move_tv.tv_sec = priv->start_move_tv.tv_sec + 1;
			priv->end_move_tv.tv_usec = mv_us - 1000000;
		} else {
			priv->end_move_tv.tv_sec = priv->start_move_tv.tv_sec;
			priv->end_move_tv.tv_usec = mv_us;
		}
	}

	return ret;
}

static const struct v4l2_ctrl_ops dw9718_vcm_ctrl_ops = {
	.g_volatile_ctrl = dw9718_get_ctrl,
	.s_ctrl = dw9718_set_ctrl,
};

static int dw9718_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	int rval;

	rval = pm_runtime_get_sync(sd->dev);
	if (rval < 0) {
		pm_runtime_put_noidle(sd->dev);
		return rval;
	}

	return 0;
}

static int dw9718_close(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	pm_runtime_put(sd->dev);

	return 0;
}

static const struct v4l2_subdev_internal_ops dw9718_int_ops = {
	.open = dw9718_open,
	.close = dw9718_close,
};

static long dw9718_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	int ret = 0;
	struct rk_cam_vcm_tim *vcm_tim;
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct dw9718_device *dw9718_dev = sd_to_dw9718_vcm(sd);

	if (cmd == RK_VIDIOC_VCM_TIMEINFO) {
		vcm_tim = (struct rk_cam_vcm_tim *)arg;

		vcm_tim->vcm_start_t.tv_sec = dw9718_dev->start_move_tv.tv_sec;
		vcm_tim->vcm_start_t.tv_usec =
				dw9718_dev->start_move_tv.tv_usec;
		vcm_tim->vcm_end_t.tv_sec = dw9718_dev->end_move_tv.tv_sec;
		vcm_tim->vcm_end_t.tv_usec = dw9718_dev->end_move_tv.tv_usec;

		dev_dbg(&client->dev,
			"dw9718_get_move_ts 0x%lx, 0x%lx, 0x%lx, 0x%lx\n",
			vcm_tim->vcm_start_t.tv_sec,
			vcm_tim->vcm_start_t.tv_usec,
			vcm_tim->vcm_end_t.tv_sec,
			vcm_tim->vcm_end_t.tv_usec);
	} else {
		dev_err(&client->dev,
			"cmd 0x%x not supported\n", cmd);
		return -EINVAL;
	}

	return ret;
}

#ifdef CONFIG_COMPAT
static long dw9718_compat_ioctl32(struct v4l2_subdev *sd,
	unsigned int cmd, unsigned long arg)
{
	struct rk_cam_vcm_tim vcm_tim;
	struct rk_cam_compat_vcm_tim compat_vcm_tim;
	struct rk_cam_compat_vcm_tim __user *p32 = compat_ptr(arg);
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	long ret;

	if (cmd == RK_VIDIOC_COMPAT_VCM_TIMEINFO) {
		ret = dw9718_ioctl(sd, RK_VIDIOC_VCM_TIMEINFO, &vcm_tim);
		compat_vcm_tim.vcm_start_t.tv_sec = vcm_tim.vcm_start_t.tv_sec;
		compat_vcm_tim.vcm_start_t.tv_usec =
				vcm_tim.vcm_start_t.tv_usec;
		compat_vcm_tim.vcm_end_t.tv_sec = vcm_tim.vcm_end_t.tv_sec;
		compat_vcm_tim.vcm_end_t.tv_usec = vcm_tim.vcm_end_t.tv_usec;

		put_user(compat_vcm_tim.vcm_start_t.tv_sec,
			&p32->vcm_start_t.tv_sec);
		put_user(compat_vcm_tim.vcm_start_t.tv_usec,
			&p32->vcm_start_t.tv_usec);
		put_user(compat_vcm_tim.vcm_end_t.tv_sec,
			&p32->vcm_end_t.tv_sec);
		put_user(compat_vcm_tim.vcm_end_t.tv_usec,
			&p32->vcm_end_t.tv_usec);
	} else {
		dev_err(&client->dev,
			"cmd 0x%x not supported\n", cmd);
		return -EINVAL;
	}

	return ret;
}
#endif

static const struct v4l2_subdev_core_ops dw9718_core_ops = {
	.ioctl = dw9718_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl32 = dw9718_compat_ioctl32
#endif
};

static const struct v4l2_subdev_ops dw9718_ops = {
	.core = &dw9718_core_ops,
};

static void dw9718_subdev_cleanup(struct dw9718_device *dw9718_dev)
{
	v4l2_device_unregister_subdev(&dw9718_dev->sd);
	v4l2_ctrl_handler_free(&dw9718_dev->ctrls_vcm);
	media_entity_cleanup(&dw9718_dev->sd.entity);
}

static int dw9718_init_controls(struct dw9718_device *priv)
{
	struct v4l2_ctrl_handler *hdl = &priv->ctrls_vcm;
	const struct v4l2_ctrl_ops *ops = &dw9718_vcm_ctrl_ops;

	v4l2_ctrl_handler_init(hdl, 1);

	v4l2_ctrl_new_std(hdl, ops, V4L2_CID_FOCUS_ABSOLUTE,
			  0, VCMDRV_MAX_LOG, 1, VCMDRV_MAX_LOG);

	if (hdl->error)
		dev_err(priv->sd.dev, "%s fail error: 0x%x\n",
			__func__, hdl->error);
	priv->sd.ctrl_handler = hdl;
	return hdl->error;
}

static int dw9718_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct device_node *np = of_node_get(client->dev.of_node);
	struct dw9718_device *dw9718_dev;
	int current_distance;
	unsigned int start_current;
	unsigned int rated_current;
	unsigned int step_mode;
	struct v4l2_subdev *sd;
	char facing[2];
	u8 value;
	int ret;

	dev_info(&client->dev, "probing...\n");

	if (of_property_read_u32(np,
		OF_CAMERA_VCMDRV_START_CURRENT,
		(unsigned int *)&start_current)) {
		start_current = DW9718_DEFAULT_START_CURRENT;
		dev_info(&client->dev, "could not get module %s\n",
			OF_CAMERA_VCMDRV_START_CURRENT);
	}
	if (of_property_read_u32(np,
		OF_CAMERA_VCMDRV_RATED_CURRENT,
		(unsigned int *)&rated_current)) {
		rated_current = DW9718_DEFAULT_RATED_CURRENT;
		dev_info(&client->dev, "could not get module %s\n",
			OF_CAMERA_VCMDRV_RATED_CURRENT);
	}
	if (of_property_read_u32(np,
		OF_CAMERA_VCMDRV_STEP_MODE,
		(unsigned int *)&step_mode)) {
		step_mode = DW9718_DEFAULT_STEP_MODE;
		dev_info(&client->dev, "could not get module %s\n",
			OF_CAMERA_VCMDRV_STEP_MODE);
	}

	dw9718_dev = devm_kzalloc(&client->dev, sizeof(*dw9718_dev),
				  GFP_KERNEL);
	if (dw9718_dev == NULL)
		return -ENOMEM;

	ret = of_property_read_u32(np, RKMODULE_CAMERA_MODULE_INDEX,
				   &dw9718_dev->module_index);
	ret |= of_property_read_string(np, RKMODULE_CAMERA_MODULE_FACING,
				       &dw9718_dev->module_facing);
	if (ret) {
		dev_err(&client->dev,
			"could not get module information!\n");
		return -EINVAL;
	}

	/* Detect device */
	ret = dw9718_i2c_rd8(client, DW9718_SACT, &value);
	if (ret < 0) {
		dev_err(&client->dev, "read DW9718_SACT failed %d\n", ret);
		return -ENXIO;
	}

	if (value != DW9718_SACT_DEFAULT_VAL &&
		value != (DW9718_SACT_MULT_TWO | DW9718_SACT_PERIOD_8_8MS))
		dev_warn(&client->dev, "incorrect ID (%02x)\n", value);

	/* Initialize according to recommended settings */
	ret = dw9718_i2c_wr8(client, DW9718_CONTROL,
			     DW9718_CONTROL_SW_LINEAR |
			     DW9718_CONTROL_S_SAC4 |
			     DW9718_CONTROL_OCP_DISABLE |
			     DW9718_CONTROL_UVLO_DISABLE);
	if (ret < 0) {
		dev_err(&client->dev, "write DW9718_CONTROL failed %d\n", ret);
		return -ENXIO;
	}
	ret = dw9718_i2c_wr8(client, DW9718_SACT,
			     DW9718_SACT_MULT_TWO |
			     DW9718_SACT_PERIOD_8_8MS);
	if (ret < 0) {
		dev_err(&client->dev, "write DW9718_SACT failed %d\n", ret);
		return -ENXIO;
	}

	v4l2_i2c_subdev_init(&dw9718_dev->sd, client, &dw9718_ops);
	dw9718_dev->sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	dw9718_dev->sd.internal_ops = &dw9718_int_ops;

	ret = dw9718_init_controls(dw9718_dev);
	if (ret)
		goto err_cleanup;

	ret = media_entity_pads_init(&dw9718_dev->sd.entity, 0, NULL);
	if (ret < 0)
		goto err_cleanup;

	sd = &dw9718_dev->sd;
	sd->entity.function = MEDIA_ENT_F_LENS;

	memset(facing, 0, sizeof(facing));
	if (strcmp(dw9718_dev->module_facing, "back") == 0)
		facing[0] = 'b';
	else
		facing[0] = 'f';

	snprintf(sd->name, sizeof(sd->name), "m%02d_%s_%s %s",
		 dw9718_dev->module_index, facing,
		 DW9718_NAME, dev_name(sd->dev));
	ret = v4l2_async_register_subdev(sd);
	if (ret)
		dev_err(&client->dev, "v4l2 async register subdev failed\n");

	current_distance = rated_current - start_current;
	current_distance = current_distance * DW9718_MAX_REG /
						DW9718_MAX_CURRENT;
	dw9718_dev->step = (current_distance + (VCMDRV_MAX_LOG - 1)) /
						VCMDRV_MAX_LOG;
	dw9718_dev->start_current = start_current * DW9718_MAX_REG /
						DW9718_MAX_CURRENT;
	dw9718_dev->rated_current = dw9718_dev->start_current +
						VCMDRV_MAX_LOG *
						dw9718_dev->step;
	dw9718_dev->step_mode     = step_mode;
	dw9718_dev->move_ms       = 0;
	dw9718_dev->current_related_pos = VCMDRV_MAX_LOG;
	dw9718_dev->start_move_tv = ns_to_timeval(ktime_get_ns());
	dw9718_dev->end_move_tv = ns_to_timeval(ktime_get_ns());

	dw9718_dev->dlc_enable = 0;
	dw9718_dev->mclk = 0;
	dw9718_dev->t_src = 0x0;

	/* Initialize default position */
	dw9718_set_pos(dw9718_dev, VCMDRV_MAX_LOG);

	dw9718_dev->vcm_movefull_t =
		dw9718_move_time(dw9718_dev, DW9718_MAX_REG);
	pm_runtime_set_active(&client->dev);
	pm_runtime_enable(&client->dev);
	pm_runtime_idle(&client->dev);

	dev_info(&client->dev, "probing successful\n");

	return 0;

err_cleanup:
	dw9718_subdev_cleanup(dw9718_dev);
	dev_err(&client->dev, "Probe failed: %d\n", ret);
	return ret;
}

static int dw9718_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct dw9718_device *dw9718_dev = sd_to_dw9718_vcm(sd);

	pm_runtime_disable(&client->dev);
	dw9718_subdev_cleanup(dw9718_dev);

	return 0;
}

static int __maybe_unused dw9718_vcm_suspend(struct device *dev)
{
	return 0;
}

static int __maybe_unused dw9718_vcm_resume(struct device *dev)
{
	return 0;
}

static const struct i2c_device_id dw9718_id_table[] = {
	{ DW9718_NAME, 0 },
	{ { 0 } }
};
MODULE_DEVICE_TABLE(i2c, dw9718_id_table);

static const struct of_device_id dw9718_of_table[] = {
	{ .compatible = "dongwoon,dw9718" },
	{ { 0 } }
};
MODULE_DEVICE_TABLE(of, dw9718_of_table);

static const struct dev_pm_ops dw9718_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(dw9718_vcm_suspend, dw9718_vcm_resume)
	SET_RUNTIME_PM_OPS(dw9718_vcm_suspend, dw9718_vcm_resume, NULL)
};

static struct i2c_driver dw9718_i2c_driver = {
	.driver = {
		.name = DW9718_NAME,
		.pm = &dw9718_pm_ops,
		.of_match_table = dw9718_of_table,
	},
	.probe = &dw9718_probe,
	.remove = &dw9718_remove,
	.id_table = dw9718_id_table,
};

module_i2c_driver(dw9718_i2c_driver);

MODULE_AUTHOR("support@friendlyarm.com");
MODULE_DESCRIPTION("DW9718 VCM driver");
MODULE_LICENSE("GPL v2");
