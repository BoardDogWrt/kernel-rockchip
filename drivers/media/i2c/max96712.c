// SPDX-License-Identifier: GPL-2.0
/*
 * max96712 GMSL2/GMSL1 to CSI-2 Deserializer driver
 *
 * Copyright (C) 2023 Rockchip Electronics Co., Ltd.
 *
 * V1.0.00 first version.
 * V1.1.00 support Frame synchronization, stream on speed optimization.
 * V1.2.00 enable lock gpio for hotplug irq.
 *         add i2c regmap for debug.
 *         support modes enable select.
 *         auto initial deskew enable configure.
 *         frame sync period enable configure.
 *
 */

#include <linux/clk.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/pinctrl/consumer.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/regulator/consumer.h>
#include <linux/sysfs.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/compat.h>
#include <linux/rk-camera-module.h>
#include <linux/of_graph.h>
#include <media/media-entity.h>
#include <media/v4l2-async.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-fwnode.h>
#include <media/v4l2-subdev.h>

#define DRIVER_VERSION			KERNEL_VERSION(1, 0x02, 0x00)

#ifndef V4L2_CID_DIGITAL_GAIN
#define V4L2_CID_DIGITAL_GAIN		V4L2_CID_GAIN
#endif

#define MAX96712_LINK_FREQ_1000MHZ	1000000000UL
#define MAX96712_XVCLK_FREQ		25000000

#define MAX96712_CHIP_ID		0xA0
#define MAX96712_REG_CHIP_ID		0x0D

#define MAX96717_CHIP_ID		0xBF
#define MAX96717_REG_CHIP_ID		0x0D

#define MAX96712_REG_CTRL_MODE		0x08a0
#define MAX96712_MODE_SW_STANDBY	0x04
#define MAX96712_MODE_STREAMING		0xa4

#define MAX96712_REMOTE_CTRL		0x0003
#define MAX96712_REMOTE_DISABLE		0xFF

#define MAX96712_LOCK_STATE_LINK_A	BIT(0)
#define MAX96712_LOCK_STATE_LINK_B	BIT(1)
#define MAX96712_LOCK_STATE_LINK_C	BIT(2)
#define MAX96712_LOCK_STATE_LINK_D	BIT(3)
#define MAX96712_LOCK_STATE_MASK	0x0F /* 0x01: Link A, 0x0F: Link A/B/C/D */

#define MAX96712_FORCE_ALL_CLOCK_EN	0 /* 1: enable, 0: disable */

#define REG_NULL			0xFFFF

#define OF_CAMERA_PINCTRL_STATE_DEFAULT	"rockchip,camera_default"
#define OF_CAMERA_PINCTRL_STATE_SLEEP	"rockchip,camera_sleep"

#define MAX96712_NAME			"max96712"

#define MAX96712_REG_VALUE_08BIT	1
#define MAX96712_REG_VALUE_16BIT	2
#define MAX96712_REG_VALUE_24BIT	3

#define MAX96712_I2C_ADDR		(0x29)
#define MAX96717_I2C_ADDR		(0x40)
#define CAMERA_I2C_ADDR			(0x30)

#define MAX96712_GET_BIT(x, bit)	((x & (1 << bit)) >> bit)
#define MAX96712_GET_BIT_M_TO_N(x, m, n)	\
		((unsigned int)(x << (31 - (n))) >> ((31 - (n)) + (m)))

enum max96712_rx_rate {
	MAX96712_RX_RATE_3GBPS = 0,
	MAX96712_RX_RATE_6GBPS,
};

static const char *const max96712_supply_names[] = {
	"avdd", /* Analog power */
	"dovdd", /* Digital I/O power */
	"dvdd", /* Digital core power */
};

#define MAX96712_NUM_SUPPLIES		ARRAY_SIZE(max96712_supply_names)

struct regval {
	u16 i2c_addr;
	u16 addr;
	u8 val;
	u16 delay;
};

struct max96712_mode {
	u32 width;
	u32 height;
	struct v4l2_fract max_fps;
	u32 hts_def;
	u32 vts_def;
	u32 exp_def;
	u32 link_freq_idx;
	u32 bus_fmt;
	u32 bpp;
	const struct regval *reg_list;
	u32 vc[PAD_MAX];
};

struct max96712 {
	struct i2c_client *client;
	struct clk *xvclk;
	struct gpio_desc *power_gpio;
	struct gpio_desc *reset_gpio;
	struct gpio_desc *pwdn_gpio;
	struct gpio_desc *lock_gpio;
	struct regulator_bulk_data supplies[MAX96712_NUM_SUPPLIES];

	struct pinctrl *pinctrl;
	struct pinctrl_state *pins_default;
	struct pinctrl_state *pins_sleep;

	struct v4l2_subdev subdev;
	struct media_pad pad;
	struct v4l2_ctrl_handler ctrl_handler;
	struct v4l2_ctrl *exposure;
	struct v4l2_ctrl *anal_gain;
	struct v4l2_ctrl *digi_gain;
	struct v4l2_ctrl *hblank;
	struct v4l2_ctrl *vblank;
	struct v4l2_ctrl *pixel_rate;
	struct v4l2_ctrl *link_freq;
	struct v4l2_ctrl *test_pattern;
	struct v4l2_fwnode_endpoint bus_cfg;

	struct mutex mutex;
	bool streaming;
	bool power_on;
	bool hot_plug;
	u8 is_reset;
	int hot_plug_irq;
	enum max96712_rx_rate rx_rate;
	u32 link_mask;
	const struct max96712_mode *supported_modes;
	const struct max96712_mode *cur_mode;
	u32 cfg_modes_num;
	u32 module_index;
	u32 auto_init_deskew_mask;
	u32 frame_sync_period;
	const char *module_facing;
	const char *module_name;
	const char *len_name;
	struct regmap *regmap;
};

static const struct regmap_config max96712_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.max_register = 0x1F17,
};

static const struct regval max96712_mipi_4lane_1920x1440_30fps[] = {
	// Link A/B/C/D all use GMSL2, and disabled
	{ 0x29, 0x0006, 0xf0, 0x00 }, // Link A/B/C/D: select GMSL2, Disabled
	// Disable MIPI CSI output
	{ 0x29, 0x040B, 0x00, 0x00 }, // CSI_OUT_EN=0, CSI output disabled
	// Increase CMU voltage
	{ 0x29, 0x06C2, 0x10, 0x0a }, // Increase CMU voltage to for wide temperature range
	// VGAHiGain
	{ 0x29, 0x14D1, 0x03, 0x00 }, // VGAHiGain
	{ 0x29, 0x15D1, 0x03, 0x00 }, // VGAHiGain
	{ 0x29, 0x16D1, 0x03, 0x00 }, // VGAHiGain
	{ 0x29, 0x17D1, 0x03, 0x0a }, // VGAHiGain
	// SSC Configuration
	{ 0x29, 0x1445, 0x00, 0x00 }, // Disable SSC
	{ 0x29, 0x1545, 0x00, 0x00 }, // Disable SSC
	{ 0x29, 0x1645, 0x00, 0x00 }, // Disable SSC
	{ 0x29, 0x1745, 0x00, 0x0a }, // Disable SSC
	// Video Pipe Selection
	{ 0x29, 0x00F0, 0x62, 0x00 }, // GMSL2 Phy A -> Pipe Z -> Pipe 0; GMSL2 Phy B -> Pipe Z -> Pipe 1
	{ 0x29, 0x00F1, 0xea, 0x00 }, // GMSL2 Phy C -> Pipe Z -> Pipe 2; GMSL2 Phy D -> Pipe Z -> Pipe 3
	{ 0x29, 0x00F4, 0x0f, 0x00 }, // Enable all 4 Pipes
	// Send YUV422, FS, and FE from Pipe 0 to Controller 1
	{ 0x29, 0x090B, 0x07, 0x00 }, // Enable 0/1/2 SRC/DST Mappings
	{ 0x29, 0x092D, 0x15, 0x00 }, // SRC/DST 0/1/2 -> CSI2 Controller 1;
	// For the following MSB 2 bits = VC, LSB 6 bits = DT
	{ 0x29, 0x090D, 0x1e, 0x00 }, // SRC0 VC = 0, DT = YUV422 8bit
	{ 0x29, 0x090E, 0x1e, 0x00 }, // DST0 VC = 0, DT = YUV422 8bit
	{ 0x29, 0x090F, 0x00, 0x00 }, // SRC1 VC = 0, DT = Frame Start
	{ 0x29, 0x0910, 0x00, 0x00 }, // DST1 VC = 0, DT = Frame Start
	{ 0x29, 0x0911, 0x01, 0x00 }, // SRC2 VC = 0, DT = Frame End
	{ 0x29, 0x0912, 0x01, 0x00 }, // DST2 VC = 0, DT = Frame End
	// Send YUV422, FS, and FE from Pipe 1 to Controller 1
	{ 0x29, 0x094B, 0x07, 0x00 }, // Enable 0/1/2 SRC/DST Mappings
	{ 0x29, 0x096D, 0x15, 0x00 }, // SRC/DST 0/1/2 -> CSI2 Controller 1;
	// For the following MSB 2 bits = VC, LSB 6 bits = DT
	{ 0x29, 0x094D, 0x1e, 0x00 }, // SRC0 VC = 0, DT = YUV422 8bit
	{ 0x29, 0x094E, 0x5e, 0x00 }, // DST0 VC = 1, DT = YUV422 8bit
	{ 0x29, 0x094F, 0x00, 0x00 }, // SRC1 VC = 0, DT = Frame Start
	{ 0x29, 0x0950, 0x40, 0x00 }, // DST1 VC = 1, DT = Frame Start
	{ 0x29, 0x0951, 0x01, 0x00 }, // SRC2 VC = 0, DT = Frame End
	{ 0x29, 0x0952, 0x41, 0x00 }, // DST2 VC = 1, DT = Frame End
	// Send YUV422, FS, and FE from Pipe 2 to Controller 1
	{ 0x29, 0x098B, 0x07, 0x00 }, // Enable 0/1/2 SRC/DST Mappings
	{ 0x29, 0x09AD, 0x15, 0x00 }, // SRC/DST 0/1/2 -> CSI2 Controller 1;
	// For the following MSB 2 bits = VC, LSB 6 bits = DT
	{ 0x29, 0x098D, 0x1e, 0x00 }, // SRC0 VC = 0, DT = YUV422 8bit
	{ 0x29, 0x098E, 0x9e, 0x00 }, // DST0 VC = 2, DT = YUV422 8bit
	{ 0x29, 0x098F, 0x00, 0x00 }, // SRC1 VC = 0, DT = Frame Start
	{ 0x29, 0x0990, 0x80, 0x00 }, // DST1 VC = 2, DT = Frame Start
	{ 0x29, 0x0991, 0x01, 0x00 }, // SRC2 VC = 0, DT = Frame End
	{ 0x29, 0x0992, 0x81, 0x00 }, // DST2 VC = 2, DT = Frame End
	// Send YUV422, FS, and FE from Pipe 3 to Controller 1
	{ 0x29, 0x09CB, 0x07, 0x00 }, // Enable 0/1/2 SRC/DST Mappings
	{ 0x29, 0x09ED, 0x15, 0x00 }, // SRC/DST 0/1/2 -> CSI2 Controller 1;
	// For the following MSB 2 bits = VC, LSB 6 bits = DT
	{ 0x29, 0x09CD, 0x1e, 0x00 }, // SRC0 VC = 0, DT = YUV422 8bit
	{ 0x29, 0x09CE, 0xde, 0x00 }, // DST0 VC = 3, DT = YUV422 8bit
	{ 0x29, 0x09CF, 0x00, 0x00 }, // SRC1 VC = 0, DT = Frame Start
	{ 0x29, 0x09D0, 0xc0, 0x00 }, // DST1 VC = 3, DT = Frame Start
	{ 0x29, 0x09D1, 0x01, 0x00 }, // SRC2 VC = 0, DT = Frame End
	{ 0x29, 0x09D2, 0xc1, 0x00 }, // DST2 VC = 3, DT = Frame End
	// MIPI PHY Setting
	{ 0x29, 0x08A0, 0x24, 0x00 }, // DPHY0 enabled as clock, MIPI PHY Mode: 2x4 mode
	// Set Lane Mapping for 4-lane port A
	{ 0x29, 0x08A3, 0xe4, 0x00 }, // PHY1 D1->D3, D0->D2; PHY0 D1->D1, D0->D0
	// Set 4 lane D-PHY, 2bit VC
	{ 0x29, 0x090A, 0xc0, 0x00 }, // MIPI PHY 0: 4 lanes, DPHY, 2bit VC
	{ 0x29, 0x094A, 0xc0, 0x00 }, // MIPI PHY 1: 4 lanes, DPHY, 2bit VC
	// Turn on MIPI PHYs
	{ 0x29, 0x08A2, 0x30, 0x00 }, // Enable MIPI PHY 0/1
	// Hold DPLL in reset (config_soft_rst_n = 0) before changing the rate
	{ 0x29, 0x1C00, 0xf4, 0x00 },
	{ 0x29, 0x1D00, 0xf4, 0x00 },
	// Set Data rate to be 2000Mbps/lane for port A and enable software override
	{ 0x29, 0x0415, 0x34, 0x00 }, // Enable freq fine tuning, 2000Mbps
	{ 0x29, 0x0418, 0x34, 0x00 }, // Enable freq fine tuning, 2000Mbps
	// Release reset to DPLL (config_soft_rst_n = 1)
	{ 0x29, 0x1C00, 0xf5, 0x00 },
	{ 0x29, 0x1D00, 0xf5, 0x00 },
	// YUV422 8bit software override for all pipes since connected GMSL1 is under parallel mode
	{ 0x29, 0x040B, 0x80, 0x00 }, // pipe 0 bpp=0x10: Datatypes = 0x22, 0x1E, 0x2E
	{ 0x29, 0x040E, 0x5e, 0x00 }, // pipe 0 DT=0x1E: YUV422 8-bit
	{ 0x29, 0x040F, 0x7e, 0x00 }, // pipe 1 DT=0x1E: YUV422 8-bit
	{ 0x29, 0x0410, 0x7a, 0x00 }, // pipe 2 DT=0x1E, pipe 3 DT=0x1E: YUV422 8-bit
	{ 0x29, 0x0411, 0x90, 0x00 }, // pipe 1 bpp=0x10: Datatypes = 0x22, 0x1E, 0x2E
	{ 0x29, 0x0412, 0x40, 0x00 }, // pipe 2 bpp=0x10, pipe 3 bpp=0x10: Datatypes = 0x22, 0x1E, 0x2E
	// Enable all links and pipes
	{ 0x29, 0x0003, 0xaa, 0x00 }, // Enable Remote Control Channel Link A/B/C/D for Port 0
	{ 0x29, 0x0006, 0xff, 0x64 }, // Enable all links and pipes
	// Serializer Setting
	{ 0x40, 0x0302, 0x10, 0x00 }, // improve CMU voltage performance to improve link robustness
	{ 0x40, 0x1417, 0x00, 0x00 }, // Errata
	{ 0x40, 0x1432, 0x7f, 0x00 },
	{ 0x29, REG_NULL, 0x00, 0x00 },
};

static const struct max96712_mode supported_modes_4lane[] = {
	{
		.width = 1920,
		.height = 1440,
		.max_fps = {
			.numerator = 10000,
			.denominator = 300000,
		},
		.reg_list = max96712_mipi_4lane_1920x1440_30fps,
		.link_freq_idx = 0,
		.bus_fmt = MEDIA_BUS_FMT_UYVY8_2X8,
		.bpp = 16,
		.vc[PAD0] = V4L2_MBUS_CSI2_CHANNEL_0,
		.vc[PAD1] = V4L2_MBUS_CSI2_CHANNEL_1,
		.vc[PAD2] = V4L2_MBUS_CSI2_CHANNEL_2,
		.vc[PAD3] = V4L2_MBUS_CSI2_CHANNEL_3,
	},
};

static const s64 link_freq_items[] = {
	MAX96712_LINK_FREQ_1000MHZ,
};

/* Write registers up to 4 at a time */
static int max96712_write_reg(struct i2c_client *client,
			u16 client_addr, u16 reg, u32 len, u8 val)
{
	u32 buf_i, val_i;
	u8 buf[6];
	u8 *val_p;
	__be32 val_be;

	dev_info(&client->dev, "addr(0x%02x) write reg(0x%04x, 0x%02x)\n", \
		client_addr, reg, val);

	if (len > 4)
		return -EINVAL;

	buf[0] = reg >> 8;
	buf[1] = reg & 0xff;

	val_be = cpu_to_be32(val);
	val_p = (u8 *)&val_be;
	buf_i = 2;
	val_i = 4 - len;

	while (val_i < 4)
		buf[buf_i++] = val_p[val_i++];
	client->addr = client_addr;

	if (i2c_master_send(client, buf, len + 2) != len + 2) {
		dev_err(&client->dev,
			"%s: writing register 0x%04x from 0x%02x failed\n",
			__func__, reg, client->addr);
		return -EIO;
	}

	return 0;
}

static int max96712_write_array(struct i2c_client *client,
				const struct regval *regs)
{
	u32 i;
	int ret = 0;

	for (i = 0; ret == 0 && regs[i].addr != REG_NULL; i++) {
		ret = max96712_write_reg(client,
					regs[i].i2c_addr, regs[i].addr,
					MAX96712_REG_VALUE_08BIT,
					regs[i].val);

		if (regs[i].delay != 0)
			msleep(regs[i].delay);
	}

	return ret;
}

/* Read registers up to 4 at a time */
static int max96712_read_reg(struct i2c_client *client,
			u16 client_addr, u16 reg, u32 len, u8 *val)
{
	struct i2c_msg msgs[2];
	u8 *data_be_p;
	__be32 data_be = 0;
	__be16 reg_addr_be = cpu_to_be16(reg);
	int ret;

	if (len > 4 || !len)
		return -EINVAL;

	client->addr = client_addr;
	data_be_p = (u8 *)&data_be;
	/* Write register address */
	msgs[0].addr = client->addr;
	msgs[0].flags = 0;
	msgs[0].len = 2;
	msgs[0].buf = (u8 *)&reg_addr_be;

	/* Read data from register */
	msgs[1].addr = client->addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = len;
	msgs[1].buf = &data_be_p[4 - len];

	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (ret != ARRAY_SIZE(msgs)) {
		dev_err(&client->dev,
			"%s: reading register 0x%x from 0x%x failed\n",
			__func__, reg, client->addr);
		return -EIO;
	}

	*val = be32_to_cpu(data_be);

#if 0
	dev_info(&client->dev, "addr(0x%02x) read reg(0x%04x, 0x%02x)\n", \
		client_addr, reg, *val);
#endif

	return 0;
}

static int __maybe_unused max96712_update_reg_bits(struct i2c_client *client,
			u16 client_addr, u16 reg, u8 mask, u8 val)
{
	u8 value;
	int ret;

	ret = max96712_read_reg(client, client_addr, reg, MAX96712_REG_VALUE_08BIT, &value);
	if (ret)
		return ret;

	value &= ~mask;
	value |= (val & mask);
	ret = max96712_write_reg(client, client_addr, reg, MAX96712_REG_VALUE_08BIT, value);
	if (ret)
		return ret;

	return 0;
}

static int max96712_check_link_lock_state(struct max96712 *max96712)
{
	struct i2c_client *client = max96712->client;
	struct device *dev = &max96712->client->dev;
	u8 id = 0, lock = 0, lock_state = 0, link_mask = 0;
	int ret, count;

	ret = max96712_read_reg(client, MAX96712_I2C_ADDR,
				MAX96712_REG_CHIP_ID,
				MAX96712_REG_VALUE_08BIT, &id);
	if (id != MAX96712_CHIP_ID) {
		dev_err(dev, "Unexpected MAX96712 chip id(%02x), ret(%d)\n", id, ret);
		return -ENODEV;
	}

	dev_info(dev, "Detected MAX96712 chipid: %02x\n", id);

	/* IF VDD = 1.2V: Enable REG_ENABLE and REG_MNL
	 *	CTRL0: Enable REG_ENABLE
	 *	CTRL2: Enable REG_MNL
	 */
	max96712_update_reg_bits(client, MAX96712_I2C_ADDR,
				0x0017, BIT(2), BIT(2));
	max96712_update_reg_bits(client, MAX96712_I2C_ADDR,
				0x0019, BIT(4), BIT(4));

	// CSI output disabled
	max96712_write_reg(client, MAX96712_I2C_ADDR, 0x040B,
				MAX96712_REG_VALUE_08BIT, 0x00);

	// All links select GMSL2 mode and disable at beginning.
	max96712_write_reg(client, MAX96712_I2C_ADDR, 0x0006,
				MAX96712_REG_VALUE_08BIT, 0xf0);

	if (max96712->rx_rate == MAX96712_RX_RATE_3GBPS) {
		// Link A ~ Link D Transmitter Rate: 187.5Mbps, Receiver Rate: 3Gbps
		max96712_write_reg(client, MAX96712_I2C_ADDR, 0x0010,
					MAX96712_REG_VALUE_08BIT, 0x11);
		max96712_write_reg(client, MAX96712_I2C_ADDR, 0x0011,
					MAX96712_REG_VALUE_08BIT, 0x11);
	} else {
		// Link A ~ Link D Transmitter Rate: 187.5Mbps, Receiver Rate: 6Gbps
		max96712_write_reg(client, MAX96712_I2C_ADDR, 0x0010,
					MAX96712_REG_VALUE_08BIT, 0x22);
		max96712_write_reg(client, MAX96712_I2C_ADDR, 0x0011,
					MAX96712_REG_VALUE_08BIT, 0x22);
	}

	// Link A ~ Link D One-Shot Reset
	link_mask = max96712->link_mask;
	max96712_write_reg(client, MAX96712_I2C_ADDR, 0x0018,
				MAX96712_REG_VALUE_08BIT, link_mask);
	max96712_write_reg(client, MAX96712_I2C_ADDR, 0x0006,
				MAX96712_REG_VALUE_08BIT, 0xf0 | link_mask);
	msleep(50);
	for (count = 0; count < 20; count++) {
		if ((lock_state & MAX96712_LOCK_STATE_LINK_A) == 0) {
			max96712_read_reg(client, MAX96712_I2C_ADDR, 0x001a,
					MAX96712_REG_VALUE_08BIT, &lock);
			if (lock & BIT(3)) {
				lock_state |= MAX96712_LOCK_STATE_LINK_A;
				dev_info(dev, "LinkA lock success: 0x%x, count: %d\n", lock, count);
			}
		}

		if ((lock_state & MAX96712_LOCK_STATE_LINK_B) == 0) {
			max96712_read_reg(client, MAX96712_I2C_ADDR, 0x000a,
					MAX96712_REG_VALUE_08BIT, &lock);
			if (lock & BIT(3)) {
				lock_state |= MAX96712_LOCK_STATE_LINK_B;
				dev_info(dev, "LinkB lock success: 0x%x, count: %d\n", lock, count);
			}
		}

		if ((lock_state & MAX96712_LOCK_STATE_LINK_C) == 0) {
			max96712_read_reg(client, MAX96712_I2C_ADDR, 0x000b,
					MAX96712_REG_VALUE_08BIT, &lock);
			if (lock & BIT(3)) {
				lock_state |= MAX96712_LOCK_STATE_LINK_C;
				dev_info(dev, "LinkC lock success: 0x%x, count: %d\n", lock, count);
			}
		}

		if ((lock_state & MAX96712_LOCK_STATE_LINK_D) == 0) {
			max96712_read_reg(client, MAX96712_I2C_ADDR, 0x000c,
					MAX96712_REG_VALUE_08BIT, &lock);
			if (lock & BIT(3)) {
				lock_state |= MAX96712_LOCK_STATE_LINK_D;
				dev_info(dev, "LinkD lock success: 0x%x, count: %d\n", lock, count);
			}
		}

		if ((lock_state & max96712->link_mask) == max96712->link_mask) {
			dev_info(dev, "All Links are locked: 0x%x\n", lock_state);
#if 0
			ret = max96712_read_reg(client, MAX96717_I2C_ADDR,
						MAX96717_REG_CHIP_ID,
						MAX96712_REG_VALUE_08BIT, &id);
			if (id != MAX96717_CHIP_ID) {
				dev_err(dev, "Unexpected MAX96717 chip id(%02x), ret(%d)\n", id, ret);
				return -ENODEV;
			}
			dev_info(dev, "Detected MAX96717 chipid: %02x\n", id);
#endif
			return 0;
		}

		msleep(10);
	}

	dev_err(dev, "Failed to detect camera link!\n");
	return -ENODEV;
}

static u8 max96712_get_lock_state(struct max96712 *max96712)
{
	struct i2c_client *client = max96712->client;
	struct device *dev = &max96712->client->dev;
	u8 lock = 0, lock_state = 0;

	if (max96712->link_mask & MAX96712_LOCK_STATE_LINK_A) {
		max96712_read_reg(client, MAX96712_I2C_ADDR, 0x001a,
				MAX96712_REG_VALUE_08BIT, &lock);
		if (lock & BIT(3)) {
			lock_state |= MAX96712_LOCK_STATE_LINK_A;
			dev_info(dev, "LinkA locked\n");
		}
	}

	if (max96712->link_mask & MAX96712_LOCK_STATE_LINK_B) {
		max96712_read_reg(client, MAX96712_I2C_ADDR, 0x000a,
				MAX96712_REG_VALUE_08BIT, &lock);
		if (lock & BIT(3)) {
			lock_state |= MAX96712_LOCK_STATE_LINK_B;
			dev_info(dev, "LinkB locked\n");
		}
	}

	if (max96712->link_mask & MAX96712_LOCK_STATE_LINK_C) {
		max96712_read_reg(client, MAX96712_I2C_ADDR, 0x000b,
				MAX96712_REG_VALUE_08BIT, &lock);
		if (lock & BIT(3)) {
			lock_state |= MAX96712_LOCK_STATE_LINK_C;
			dev_info(dev, "LinkC locked\n");
		}
	}

	if (max96712->link_mask & MAX96712_LOCK_STATE_LINK_D) {
		max96712_read_reg(client, MAX96712_I2C_ADDR, 0x000c,
				MAX96712_REG_VALUE_08BIT, &lock);
		if (lock & BIT(3)) {
			lock_state |= MAX96712_LOCK_STATE_LINK_D;
			dev_info(dev, "LinkD locked\n");
		}
	}

	return lock_state;
}

static irqreturn_t max96712_hot_plug_detect_irq_handler(int irq, void *dev_id)
{
	struct max96712 *max96712 = dev_id;
	struct device *dev = &max96712->client->dev;
	u8 lock_state = 0;

	if (max96712->streaming) {
		lock_state = max96712_get_lock_state(max96712);
		if (lock_state == max96712->link_mask) {
			dev_info(dev, "serializer plug in, lock_state = 0x%02x\n", lock_state);
		} else {
			dev_info(dev, "serializer plug out, lock_state = 0x%02x\n", lock_state);
		}
	}

	return IRQ_HANDLED;
}

static int max96712_auto_init_deskew(struct i2c_client *client, u32 deskew_mask)
{
	int ret = 0;

	dev_info(&client->dev, "Auto initial deskew: deskew_mask = 0x%02x\n", deskew_mask);

	// D-PHY Deskew Initial Calibration Control
	if (deskew_mask & BIT(0)) // MIPI PHY0
		ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
			0x0903,
			MAX96712_REG_VALUE_08BIT,
			0x80);

	if (deskew_mask & BIT(1)) // MIPI PHY1
		ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
			0x0943,
			MAX96712_REG_VALUE_08BIT,
			0x80);

	if (deskew_mask & BIT(2)) // MIPI PHY2
		ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
			0x0983,
			MAX96712_REG_VALUE_08BIT,
			0x80);

	if (deskew_mask & BIT(3)) // MIPI PHY3
		ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
			0x09C3,
			MAX96712_REG_VALUE_08BIT,
			0x80);

	return ret;
}

static int max96712_frame_sync_period(struct i2c_client *client, u32 period)
{
	u32 pclk, fsync_peroid;
	u8 fsync_peroid_h, fsync_peroid_m, fsync_peroid_l;
	int ret = 0;

	if (period == 0)
		return 0;

	dev_info(&client->dev, "Frame sync period = %d\n", period);

#if 1 // TODO: Sensor
	// SC320AT slave mode enable
	ret |= max96712_write_reg(client, 0x30,
		0x3222,
		MAX96712_REG_VALUE_08BIT,
		0x01);
	// Increase the allowable error range of the trigger signal
	ret |= max96712_write_reg(client, 0x30,
		0x32e2,
		MAX96712_REG_VALUE_08BIT,
		0x19);
#endif

	// Master link Video 0 for frame sync generation
	ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
		0x04A2,
		MAX96712_REG_VALUE_08BIT,
		0x00);
	// Disable Vsync-Fsync overlap window
	ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
		0x04AA,
		MAX96712_REG_VALUE_08BIT,
		0x00);
	ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
		0x04AB,
		MAX96712_REG_VALUE_08BIT,
		0x00);

	// Set FSYNC period to 25M/30 clock cycles. PCLK = 25MHz. Sync freq = 30Hz
	pclk = 25 * 1000 * 1000;
	fsync_peroid = DIV_ROUND_UP(pclk, period) - 1;
	fsync_peroid_l = (fsync_peroid >> 0) & 0xFF;
	fsync_peroid_m = (fsync_peroid >> 8) & 0xFF;
	fsync_peroid_h = (fsync_peroid >> 16) & 0xFF;
	dev_info(&client->dev, "Frame sync period: H = 0x%02x, M = 0x%02x, L = 0x%02x\n",
			fsync_peroid_h, fsync_peroid_m, fsync_peroid_l);
	// FSYNC_PERIOD_H
	ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
		0x04A7,
		MAX96712_REG_VALUE_08BIT,
		fsync_peroid_h);
	// FSYNC_PERIOD_M
	ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
		0x04A6,
		MAX96712_REG_VALUE_08BIT,
		fsync_peroid_m);
	// FSYNC_PERIOD_L
	ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
		0x04A5,
		MAX96712_REG_VALUE_08BIT,
		fsync_peroid_l);

	// FSYNC is GMSL2 type, use osc for fsync, include all links/pipes in fsync gen
	ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
		0x04AF,
		MAX96712_REG_VALUE_08BIT,
		0xcf);

	// FSYNC_TX_ID: set 4 to match MFP4 on serializer side
	ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
		0x04B1,
		MAX96712_REG_VALUE_08BIT,
		0x20);

#if 1 // TODO: Serializer
	// Enable GPIO_RX_EN on serializer MFP4
	ret |= max96712_write_reg(client, 0x40,
		0x02CA,
		MAX96712_REG_VALUE_08BIT,
		0x84);
#endif

	// MFP2, VS not gen internally, GPIO not used to gen fsync, manual mode
	ret |= max96712_write_reg(client, MAX96712_I2C_ADDR,
		0x04A0,
		MAX96712_REG_VALUE_08BIT,
		0x04);

	return ret;
}

static int max96712_mipi_enable(struct i2c_client *client, bool enable)
{
	int ret = 0;

	if (enable) {
#if MAX96712_FORCE_ALL_CLOCK_EN
		// Force all MIPI clocks running
		ret |= max96712_update_reg_bits(client,
				MAX96712_I2C_ADDR,
				0x08A0, BIT(7), BIT(7));
#endif
		// CSI output enabled
		ret |= max96712_update_reg_bits(client,
				MAX96712_I2C_ADDR,
				0x040B, BIT(1), BIT(1));
	} else {
#if MAX96712_FORCE_ALL_CLOCK_EN
		// Normal mode
		ret |= max96712_update_reg_bits(client,
				MAX96712_I2C_ADDR,
				0x08A0, BIT(7), 0x00);
#endif
		// CSI output disabled
		ret |= max96712_update_reg_bits(client,
				MAX96712_I2C_ADDR,
				0x040B, BIT(1), 0x00);
	}

	return ret;
}

static int max96712_get_reso_dist(const struct max96712_mode *mode,
				  struct v4l2_mbus_framefmt *framefmt)
{
	return abs(mode->width - framefmt->width) +
	       abs(mode->height - framefmt->height);
}

static const struct max96712_mode *
max96712_find_best_fit(struct max96712 *max96712, struct v4l2_subdev_format *fmt)
{
	struct v4l2_mbus_framefmt *framefmt = &fmt->format;
	int dist;
	int cur_best_fit = 0;
	int cur_best_fit_dist = -1;
	unsigned int i;

	for (i = 0; i < max96712->cfg_modes_num; i++) {
		dist = max96712_get_reso_dist(&max96712->supported_modes[i], framefmt);
		if ((cur_best_fit_dist == -1 || dist < cur_best_fit_dist)
				&& (max96712->supported_modes[i].bus_fmt == framefmt->code)) {
			cur_best_fit_dist = dist;
			cur_best_fit = i;
		}
	}

	return &max96712->supported_modes[cur_best_fit];
}

static int max96712_set_fmt(struct v4l2_subdev *sd,
			    struct v4l2_subdev_state *sd_state,
			    struct v4l2_subdev_format *fmt)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);
	const struct max96712_mode *mode;
	u64 pixel_rate = 0;
	u8 data_lanes;

	mutex_lock(&max96712->mutex);

	mode = max96712_find_best_fit(max96712, fmt);

	fmt->format.code = mode->bus_fmt;
	fmt->format.width = mode->width;
	fmt->format.height = mode->height;
	fmt->format.field = V4L2_FIELD_NONE;
	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
		*v4l2_subdev_get_try_format(sd, sd_state, fmt->pad) = fmt->format;
#else
		mutex_unlock(&max96712->mutex);
		return -ENOTTY;
#endif
	} else {
		if (max96712->streaming) {
			mutex_unlock(&max96712->mutex);
			return -EBUSY;
		}

		max96712->cur_mode = mode;

		__v4l2_ctrl_s_ctrl(max96712->link_freq, mode->link_freq_idx);
		/* pixel rate = link frequency * 2 * lanes / BITS_PER_SAMPLE */
		data_lanes = max96712->bus_cfg.bus.mipi_csi2.num_data_lanes;
		pixel_rate = (u32)link_freq_items[mode->link_freq_idx] / mode->bpp * 2 * data_lanes;
		__v4l2_ctrl_s_ctrl_int64(max96712->pixel_rate, pixel_rate);

		dev_info(&max96712->client->dev, "mipi_freq_idx = %d\n", mode->link_freq_idx);
		dev_info(&max96712->client->dev, "pixel_rate = %lld\n", pixel_rate);
	}

	mutex_unlock(&max96712->mutex);

	return 0;
}

static int max96712_get_fmt(struct v4l2_subdev *sd,
			    struct v4l2_subdev_state *sd_state,
			    struct v4l2_subdev_format *fmt)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);
	const struct max96712_mode *mode = max96712->cur_mode;

	mutex_lock(&max96712->mutex);
	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
		fmt->format = *v4l2_subdev_get_try_format(sd, sd_state, fmt->pad);
#else
		mutex_unlock(&max96712->mutex);
		return -ENOTTY;
#endif
	} else {
		fmt->format.width = mode->width;
		fmt->format.height = mode->height;
		fmt->format.code = mode->bus_fmt;
		fmt->format.field = V4L2_FIELD_NONE;
		if (fmt->pad < PAD_MAX && fmt->pad >= PAD0)
			fmt->reserved[0] = mode->vc[fmt->pad];
		else
			fmt->reserved[0] = mode->vc[PAD0];
	}
	mutex_unlock(&max96712->mutex);

	return 0;
}

static int max96712_enum_mbus_code(struct v4l2_subdev *sd,
				   struct v4l2_subdev_state *sd_state,
				   struct v4l2_subdev_mbus_code_enum *code)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);
	const struct max96712_mode *mode = max96712->cur_mode;

	if (code->index != 0)
		return -EINVAL;
	code->code = mode->bus_fmt;

	return 0;
}

static int max96712_enum_frame_sizes(struct v4l2_subdev *sd,
				     struct v4l2_subdev_state *sd_state,
				     struct v4l2_subdev_frame_size_enum *fse)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);

	if (fse->index >= max96712->cfg_modes_num)
		return -EINVAL;

	if (fse->code != max96712->supported_modes[fse->index].bus_fmt)
		return -EINVAL;

	fse->min_width  = max96712->supported_modes[fse->index].width;
	fse->max_width  = max96712->supported_modes[fse->index].width;
	fse->max_height = max96712->supported_modes[fse->index].height;
	fse->min_height = max96712->supported_modes[fse->index].height;

	return 0;
}

static int max96712_g_frame_interval(struct v4l2_subdev *sd,
				     struct v4l2_subdev_frame_interval *fi)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);
	const struct max96712_mode *mode = max96712->cur_mode;

	mutex_lock(&max96712->mutex);
	fi->interval = mode->max_fps;
	mutex_unlock(&max96712->mutex);

	return 0;
}

static void max96712_get_module_inf(struct max96712 *max96712,
				    struct rkmodule_inf *inf)
{
	memset(inf, 0, sizeof(*inf));
	strscpy(inf->base.sensor, MAX96712_NAME, sizeof(inf->base.sensor));
	strscpy(inf->base.module, max96712->module_name,
		sizeof(inf->base.module));
	strscpy(inf->base.lens, max96712->len_name, sizeof(inf->base.lens));
}

static void
max96712_get_vicap_rst_inf(struct max96712 *max96712,
			   struct rkmodule_vicap_reset_info *rst_info)
{
	struct i2c_client *client = max96712->client;

	rst_info->is_reset = max96712->hot_plug;
	max96712->hot_plug = false;
	rst_info->src = RKCIF_RESET_SRC_ERR_HOTPLUG;
	dev_info(&client->dev, "%s: rst_info->is_reset:%d.\n", __func__,
		 rst_info->is_reset);
}

static void
max96712_set_vicap_rst_inf(struct max96712 *max96712,
			   struct rkmodule_vicap_reset_info rst_info)
{
	max96712->is_reset = rst_info.is_reset;
}

static long max96712_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);
	long ret = 0;
	u32 stream = 0;

	switch (cmd) {
	case RKMODULE_GET_MODULE_INFO:
		max96712_get_module_inf(max96712, (struct rkmodule_inf *)arg);
		break;
	case RKMODULE_SET_QUICK_STREAM:
		stream = *((u32 *)arg);

		if (stream)
			ret = max96712_mipi_enable(max96712->client, true);
		else
			ret = max96712_mipi_enable(max96712->client, false);
		break;
	case RKMODULE_GET_VICAP_RST_INFO:
		max96712_get_vicap_rst_inf(
			max96712, (struct rkmodule_vicap_reset_info *)arg);
		break;
	case RKMODULE_SET_VICAP_RST_INFO:
		max96712_set_vicap_rst_inf(
			max96712, *(struct rkmodule_vicap_reset_info *)arg);
		break;
	case RKMODULE_GET_START_STREAM_SEQ:
		break;
	default:
		ret = -ENOIOCTLCMD;
		break;
	}

	return ret;
}

#ifdef CONFIG_COMPAT
static long max96712_compat_ioctl32(struct v4l2_subdev *sd, unsigned int cmd,
				    unsigned long arg)
{
	void __user *up = compat_ptr(arg);
	struct rkmodule_inf *inf;
	struct rkmodule_awb_cfg *cfg;
	struct rkmodule_vicap_reset_info *vicap_rst_inf;
	long ret = 0;
	int *seq;
	u32 stream = 0;

	switch (cmd) {
	case RKMODULE_GET_MODULE_INFO:
		inf = kzalloc(sizeof(*inf), GFP_KERNEL);
		if (!inf) {
			ret = -ENOMEM;
			return ret;
		}

		ret = max96712_ioctl(sd, cmd, inf);
		if (!ret) {
			ret = copy_to_user(up, inf, sizeof(*inf));
			if (ret)
				ret = -EFAULT;
		}
		kfree(inf);
		break;
	case RKMODULE_AWB_CFG:
		cfg = kzalloc(sizeof(*cfg), GFP_KERNEL);
		if (!cfg) {
			ret = -ENOMEM;
			return ret;
		}

		ret = copy_from_user(cfg, up, sizeof(*cfg));
		if (!ret)
			ret = max96712_ioctl(sd, cmd, cfg);
		else
			ret = -EFAULT;
		kfree(cfg);
		break;
	case RKMODULE_GET_VICAP_RST_INFO:
		vicap_rst_inf = kzalloc(sizeof(*vicap_rst_inf), GFP_KERNEL);
		if (!vicap_rst_inf) {
			ret = -ENOMEM;
			return ret;
		}

		ret = max96712_ioctl(sd, cmd, vicap_rst_inf);
		if (!ret) {
			ret = copy_to_user(up, vicap_rst_inf,
					   sizeof(*vicap_rst_inf));
			if (ret)
				ret = -EFAULT;
		}
		kfree(vicap_rst_inf);
		break;
	case RKMODULE_SET_VICAP_RST_INFO:
		vicap_rst_inf = kzalloc(sizeof(*vicap_rst_inf), GFP_KERNEL);
		if (!vicap_rst_inf) {
			ret = -ENOMEM;
			return ret;
		}

		ret = copy_from_user(vicap_rst_inf, up, sizeof(*vicap_rst_inf));
		if (!ret)
			ret = max96712_ioctl(sd, cmd, vicap_rst_inf);
		else
			ret = -EFAULT;
		kfree(vicap_rst_inf);
		break;
	case RKMODULE_GET_START_STREAM_SEQ:
		seq = kzalloc(sizeof(*seq), GFP_KERNEL);
		if (!seq) {
			ret = -ENOMEM;
			return ret;
		}

		ret = max96712_ioctl(sd, cmd, seq);
		if (!ret) {
			ret = copy_to_user(up, seq, sizeof(*seq));
			if (ret)
				ret = -EFAULT;
		}
		kfree(seq);
		break;
	case RKMODULE_SET_QUICK_STREAM:
		ret = copy_from_user(&stream, up, sizeof(u32));
		if (!ret)
			ret = max96712_ioctl(sd, cmd, &stream);
		else
			ret = -EFAULT;
		break;
	default:
		ret = -ENOIOCTLCMD;
		break;
	}

	return ret;
}
#endif

static int __max96712_start_stream(struct max96712 *max96712)
{
	int ret;

	ret = max96712_check_link_lock_state(max96712);
	if (ret)
		return ret;

	if (max96712->hot_plug_irq > 0)
		enable_irq(max96712->hot_plug_irq);

	ret = max96712_write_array(max96712->client,
				   max96712->cur_mode->reg_list);
	if (ret)
		return ret;

	if (max96712->auto_init_deskew_mask != 0) {
		ret = max96712_auto_init_deskew(max96712->client,
					max96712->auto_init_deskew_mask);
		if (ret)
			return ret;
	}

	if (max96712->frame_sync_period != 0) {
		ret = max96712_frame_sync_period(max96712->client,
					max96712->frame_sync_period);
		if (ret)
			return ret;
	}

	/* In case these controls are set before streaming */
	mutex_unlock(&max96712->mutex);
	ret = v4l2_ctrl_handler_setup(&max96712->ctrl_handler);
	mutex_lock(&max96712->mutex);
	if (ret)
		return ret;

	return max96712_mipi_enable(max96712->client, true);

}

static int __max96712_stop_stream(struct max96712 *max96712)
{
	if (max96712->hot_plug_irq > 0)
		disable_irq(max96712->hot_plug_irq);

	return max96712_mipi_enable(max96712->client, false);
}

static int max96712_s_stream(struct v4l2_subdev *sd, int on)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);
	struct i2c_client *client = max96712->client;
	int ret = 0;

	dev_info(&client->dev, "%s: on: %d, %dx%d@%d\n", __func__, on,
		max96712->cur_mode->width, max96712->cur_mode->height,
		DIV_ROUND_CLOSEST(max96712->cur_mode->max_fps.denominator,
				  max96712->cur_mode->max_fps.numerator));

	mutex_lock(&max96712->mutex);
	on = !!on;
	if (on == max96712->streaming)
		goto unlock_and_return;

	if (on) {
		ret = pm_runtime_get_sync(&client->dev);
		if (ret < 0) {
			pm_runtime_put_noidle(&client->dev);
			goto unlock_and_return;
		}

		ret = __max96712_start_stream(max96712);
		if (ret) {
			v4l2_err(sd, "start stream failed while write regs\n");
			pm_runtime_put(&client->dev);
			goto unlock_and_return;
		}
	} else {
		__max96712_stop_stream(max96712);
		pm_runtime_put(&client->dev);
	}

	max96712->streaming = on;

unlock_and_return:
	mutex_unlock(&max96712->mutex);

	return ret;
}

static int max96712_s_power(struct v4l2_subdev *sd, int on)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);
	struct i2c_client *client = max96712->client;
	int ret = 0;

	mutex_lock(&max96712->mutex);

	/* If the power state is not modified - no work to do. */
	if (max96712->power_on == !!on)
		goto unlock_and_return;

	if (on) {
		ret = pm_runtime_get_sync(&client->dev);
		if (ret < 0) {
			pm_runtime_put_noidle(&client->dev);
			goto unlock_and_return;
		}

		max96712->power_on = true;
	} else {
		pm_runtime_put(&client->dev);
		max96712->power_on = false;
	}

unlock_and_return:
	mutex_unlock(&max96712->mutex);

	return ret;
}

/* Calculate the delay in us by clock rate and clock cycles */
static inline u32 max96712_cal_delay(u32 cycles)
{
	return DIV_ROUND_UP(cycles, MAX96712_XVCLK_FREQ / 1000 / 1000);
}

static int __max96712_power_on(struct max96712 *max96712)
{
	int ret;
	u32 delay_us;
	struct device *dev = &max96712->client->dev;

	if (!IS_ERR(max96712->power_gpio))
		gpiod_set_value_cansleep(max96712->power_gpio, 1);

	usleep_range(1000, 2000);

	if (!IS_ERR_OR_NULL(max96712->pins_default)) {
		ret = pinctrl_select_state(max96712->pinctrl,
					   max96712->pins_default);
		if (ret < 0)
			dev_err(dev, "could not set pins\n");
	}

	if (!IS_ERR(max96712->reset_gpio))
		gpiod_set_value_cansleep(max96712->reset_gpio, 0);

	ret = regulator_bulk_enable(MAX96712_NUM_SUPPLIES, max96712->supplies);
	if (ret < 0) {
		dev_err(dev, "Failed to enable regulators\n");
		goto disable_clk;
	}
	if (!IS_ERR(max96712->reset_gpio))
		gpiod_set_value_cansleep(max96712->reset_gpio, 1);

	usleep_range(500, 1000);
	if (!IS_ERR(max96712->pwdn_gpio))
		gpiod_set_value_cansleep(max96712->pwdn_gpio, 1);

	/* 8192 cycles prior to first SCCB transaction */
	delay_us = max96712_cal_delay(8192);
	usleep_range(delay_us, delay_us * 2);

	return 0;

disable_clk:
	clk_disable_unprepare(max96712->xvclk);

	return ret;
}

static void __max96712_power_off(struct max96712 *max96712)
{
	int ret;
	struct device *dev = &max96712->client->dev;

	if (!IS_ERR(max96712->pwdn_gpio))
		gpiod_set_value_cansleep(max96712->pwdn_gpio, 0);
	clk_disable_unprepare(max96712->xvclk);

	if (!IS_ERR(max96712->reset_gpio))
		gpiod_set_value_cansleep(max96712->reset_gpio, 0);

	if (!IS_ERR_OR_NULL(max96712->pins_sleep)) {
		ret = pinctrl_select_state(max96712->pinctrl,
					   max96712->pins_sleep);
		if (ret < 0)
			dev_dbg(dev, "could not set pins\n");
	}
	if (!IS_ERR(max96712->power_gpio))
		gpiod_set_value_cansleep(max96712->power_gpio, 0);

	regulator_bulk_disable(MAX96712_NUM_SUPPLIES, max96712->supplies);
}

static int max96712_runtime_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);

	return __max96712_power_on(max96712);
}

static int max96712_runtime_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);

	__max96712_power_off(max96712);

	return 0;
}

#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
static int max96712_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);
	struct v4l2_mbus_framefmt *try_fmt =
		v4l2_subdev_get_try_format(sd, fh->state, 0);
	const struct max96712_mode *def_mode = &max96712->supported_modes[0];

	mutex_lock(&max96712->mutex);
	/* Initialize try_fmt */
	try_fmt->width = def_mode->width;
	try_fmt->height = def_mode->height;
	try_fmt->code = def_mode->bus_fmt;
	try_fmt->field = V4L2_FIELD_NONE;

	mutex_unlock(&max96712->mutex);
	/* No crop or compose */

	return 0;
}
#endif

static int
max96712_enum_frame_interval(struct v4l2_subdev *sd,
			     struct v4l2_subdev_state *sd_state,
			     struct v4l2_subdev_frame_interval_enum *fie)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);

	if (fie->index >= max96712->cfg_modes_num)
		return -EINVAL;

	fie->code = max96712->supported_modes[fie->index].bus_fmt;
	fie->width = max96712->supported_modes[fie->index].width;
	fie->height = max96712->supported_modes[fie->index].height;
	fie->interval = max96712->supported_modes[fie->index].max_fps;

	return 0;
}

static int max96712_g_mbus_config(struct v4l2_subdev *sd, unsigned int pad,
				  struct v4l2_mbus_config *config)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);
	u32 val = 0;
	u8 data_lanes = max96712->bus_cfg.bus.mipi_csi2.num_data_lanes;

	val |= V4L2_MBUS_CSI2_CONTINUOUS_CLOCK;
	val |= 1 << (data_lanes - 1);
	switch (data_lanes) {
	case 4:
		val |= V4L2_MBUS_CSI2_CHANNEL_3;
		fallthrough;
	case 3:
		val |= V4L2_MBUS_CSI2_CHANNEL_2;
		fallthrough;
	case 2:
		val |= V4L2_MBUS_CSI2_CHANNEL_1;
		fallthrough;
	case 1:
	default:
		val |= V4L2_MBUS_CSI2_CHANNEL_0;
		break;
	}

	config->type = V4L2_MBUS_CSI2_DPHY;
	config->flags = val;

	return 0;
}

static int max96712_get_selection(struct v4l2_subdev *sd,
				  struct v4l2_subdev_state *sd_state,
				  struct v4l2_subdev_selection *sel)
{
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);

	if (sel->target == V4L2_SEL_TGT_CROP_BOUNDS) {
		sel->r.left = 0;
		sel->r.width = max96712->cur_mode->width;
		sel->r.top = 0;
		sel->r.height = max96712->cur_mode->height;
		return 0;
	}

	return -EINVAL;
}

static const struct dev_pm_ops max96712_pm_ops = { SET_RUNTIME_PM_OPS(
	max96712_runtime_suspend, max96712_runtime_resume, NULL) };

#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
static const struct v4l2_subdev_internal_ops max96712_internal_ops = {
	.open = max96712_open,
};
#endif

static const struct v4l2_subdev_core_ops max96712_core_ops = {
	.s_power = max96712_s_power,
	.ioctl = max96712_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl32 = max96712_compat_ioctl32,
#endif
};

static const struct v4l2_subdev_video_ops max96712_video_ops = {
	.s_stream = max96712_s_stream,
	.g_frame_interval = max96712_g_frame_interval,
};

static const struct v4l2_subdev_pad_ops max96712_pad_ops = {
	.enum_mbus_code = max96712_enum_mbus_code,
	.enum_frame_size = max96712_enum_frame_sizes,
	.enum_frame_interval = max96712_enum_frame_interval,
	.get_fmt = max96712_get_fmt,
	.set_fmt = max96712_set_fmt,
	.get_selection = max96712_get_selection,
	.get_mbus_config = max96712_g_mbus_config,
};

static const struct v4l2_subdev_ops max96712_subdev_ops = {
	.core = &max96712_core_ops,
	.video = &max96712_video_ops,
	.pad = &max96712_pad_ops,
};

static int max96712_initialize_controls(struct max96712 *max96712)
{
	const struct max96712_mode *mode;
	struct v4l2_ctrl_handler *handler;
	u64 pixel_rate;
	u8 data_lanes;
	int ret;

	handler = &max96712->ctrl_handler;

	mode = max96712->cur_mode;
	ret = v4l2_ctrl_handler_init(handler, 2);
	if (ret)
		return ret;
	handler->lock = &max96712->mutex;

	max96712->link_freq = v4l2_ctrl_new_int_menu(handler, NULL,
				V4L2_CID_LINK_FREQ,
				ARRAY_SIZE(link_freq_items) - 1, 0,
				link_freq_items);
	__v4l2_ctrl_s_ctrl(max96712->link_freq, mode->link_freq_idx);
	dev_info(&max96712->client->dev, "mipi_freq_idx = %d\n", mode->link_freq_idx);

	/* pixel rate = link frequency * 2 * lanes / BITS_PER_SAMPLE */
	data_lanes = max96712->bus_cfg.bus.mipi_csi2.num_data_lanes;
	pixel_rate = (u32)link_freq_items[mode->link_freq_idx] / mode->bpp * 2 * data_lanes;
	max96712->pixel_rate =
		v4l2_ctrl_new_std(handler, NULL, V4L2_CID_PIXEL_RATE, 0,
				  pixel_rate, 1, pixel_rate);
	dev_info(&max96712->client->dev, "pixel_rate = %lld\n", pixel_rate);

	if (handler->error) {
		ret = handler->error;
		dev_err(&max96712->client->dev, "Failed to init controls(%d)\n",
			ret);
		goto err_free_handler;
	}

	max96712->subdev.ctrl_handler = handler;

	return 0;

err_free_handler:
	v4l2_ctrl_handler_free(handler);

	return ret;
}

static int max96712_configure_regulators(struct max96712 *max96712)
{
	unsigned int i;

	for (i = 0; i < MAX96712_NUM_SUPPLIES; i++)
		max96712->supplies[i].supply = max96712_supply_names[i];

	return devm_regulator_bulk_get(&max96712->client->dev,
				       MAX96712_NUM_SUPPLIES,
				       max96712->supplies);
}

static int max96712_parse_dt(struct max96712 *max96712)
{
	struct device *dev = &max96712->client->dev;
	struct device_node *node = dev->of_node;
	u8 mipi_data_lanes = max96712->bus_cfg.bus.mipi_csi2.num_data_lanes;
	int ret = 0;

	/* max96712 link Receiver Rate */
	ret = of_property_read_u32(node, "link-rx-rate",
				   &max96712->rx_rate);
	if (ret)
		max96712->rx_rate = MAX96712_RX_RATE_6GBPS;
	else
		dev_info(dev, "link-rx-rate property: %d\n", max96712->rx_rate);
	dev_info(dev, "max96712 link receiver rate: %d\n", max96712->rx_rate);

	/* max96712 link mask: bit0 - LinkA, bit1 - LinkB, bit2 - LinkC, bit3 - LinkD */
	ret = of_property_read_u32(node, "link-mask",
				   &max96712->link_mask);
	if (ret) {
		/* default link mask */
		if (mipi_data_lanes == 4)
			max96712->link_mask = 0x0F; /* Link A/B/C/D */
		else
			max96712->link_mask = 0x03; /* Link A/B */
	} else {
		dev_info(dev, "link-mask property: %d\n", max96712->link_mask);
	}
	dev_info(dev, "max96712 link mask: 0x%02x\n", max96712->link_mask);

	/* auto initial deskew mask */
	ret = of_property_read_u32(node, "auto-init-deskew-mask",
			   &max96712->auto_init_deskew_mask);
	if (ret)
		max96712->auto_init_deskew_mask = 0x0F; // 0x0F: default enable all
	dev_info(dev, "max96712 auto init deskew mask: 0x%02x\n", max96712->auto_init_deskew_mask);

	/* FSYNC period config */
	ret = of_property_read_u32(node, "frame-sync-period",
			   &max96712->frame_sync_period);
	if (ret)
		max96712->frame_sync_period = 0; // 0: disable (default)
	dev_info(dev, "max96712 frame sync period: %d\n", max96712->frame_sync_period);

	return 0;
}

static int max96712_probe(struct i2c_client *client,
			  const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct device_node *node = dev->of_node;
	struct max96712 *max96712;
	struct v4l2_subdev *sd;
	struct device_node *endpoint;
	char facing[2];
	u8 mipi_data_lanes;
	int ret;

	dev_info(dev, "driver version: %02x.%02x.%02x", DRIVER_VERSION >> 16,
		 (DRIVER_VERSION & 0xff00) >> 8, DRIVER_VERSION & 0x00ff);

	max96712 = devm_kzalloc(dev, sizeof(*max96712), GFP_KERNEL);
	if (!max96712)
		return -ENOMEM;

	ret = of_property_read_u32(node, RKMODULE_CAMERA_MODULE_INDEX,
				   &max96712->module_index);
	ret |= of_property_read_string(node, RKMODULE_CAMERA_MODULE_FACING,
				       &max96712->module_facing);
	ret |= of_property_read_string(node, RKMODULE_CAMERA_MODULE_NAME,
				       &max96712->module_name);
	ret |= of_property_read_string(node, RKMODULE_CAMERA_LENS_NAME,
				       &max96712->len_name);
	if (ret) {
		dev_err(dev, "could not get module information!\n");
		return -EINVAL;
	}

	max96712->regmap = devm_regmap_init_i2c(client, &max96712_regmap_config);
	if (IS_ERR(max96712->regmap)) {
		dev_err(dev, "Failed to regmap initialize I2C\n");
		return PTR_ERR(max96712->regmap);
	}

	max96712->client = client;
	i2c_set_clientdata(client, max96712);

	endpoint = of_graph_get_next_endpoint(dev->of_node, NULL);
	if (!endpoint) {
		dev_err(dev, "Failed to get endpoint\n");
		return -EINVAL;
	}

	ret = v4l2_fwnode_endpoint_parse(of_fwnode_handle(endpoint),
		&max96712->bus_cfg);
	if (ret) {
		dev_err(dev, "Failed to get bus config\n");
		return -EINVAL;
	}
	mipi_data_lanes = max96712->bus_cfg.bus.mipi_csi2.num_data_lanes;
	dev_info(dev, "max96712 data lanes %d\n", mipi_data_lanes);

	if (mipi_data_lanes == 4) {
		max96712->supported_modes = supported_modes_4lane;
		max96712->cfg_modes_num = ARRAY_SIZE(supported_modes_4lane);
	} else {
		dev_err(dev, "Not support mipi data lane: %d\n", mipi_data_lanes);
		return -EINVAL;
	}
	max96712->cur_mode = &max96712->supported_modes[0];

	max96712->power_gpio = devm_gpiod_get(dev, "power", GPIOD_OUT_LOW);
	if (IS_ERR(max96712->power_gpio))
		dev_warn(dev, "Failed to get power-gpios, maybe no use\n");

	max96712->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_LOW);
	if (IS_ERR(max96712->reset_gpio))
		dev_warn(dev, "Failed to get reset-gpios\n");

	max96712->pwdn_gpio = devm_gpiod_get(dev, "pwdn", GPIOD_OUT_LOW);
	if (IS_ERR(max96712->pwdn_gpio))
		dev_warn(dev, "Failed to get pwdn-gpios\n");

	max96712->lock_gpio = devm_gpiod_get(dev, "lock", GPIOD_IN);
	if (IS_ERR(max96712->lock_gpio))
		dev_warn(dev, "Failed to get lock-gpios\n");

	ret = max96712_configure_regulators(max96712);
	if (ret) {
		dev_err(dev, "Failed to get power regulators\n");
		return ret;
	}

	max96712->pinctrl = devm_pinctrl_get(dev);
	if (!IS_ERR(max96712->pinctrl)) {
		max96712->pins_default = pinctrl_lookup_state(
			max96712->pinctrl, OF_CAMERA_PINCTRL_STATE_DEFAULT);
		if (IS_ERR(max96712->pins_default))
			dev_err(dev, "could not get default pinstate\n");

		max96712->pins_sleep = pinctrl_lookup_state(
			max96712->pinctrl, OF_CAMERA_PINCTRL_STATE_SLEEP);
		if (IS_ERR(max96712->pins_sleep))
			dev_err(dev, "could not get sleep pinstate\n");
	}

	max96712_parse_dt(max96712);

	mutex_init(&max96712->mutex);

	sd = &max96712->subdev;
	v4l2_i2c_subdev_init(sd, client, &max96712_subdev_ops);
	ret = max96712_initialize_controls(max96712);
	if (ret)
		goto err_destroy_mutex;

	ret = __max96712_power_on(max96712);
	if (ret)
		goto err_free_handler;

	ret = max96712_check_link_lock_state(max96712);
	if (ret)
		goto err_power_off;

#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
	sd->internal_ops = &max96712_internal_ops;
	sd->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
#endif
#if defined(CONFIG_MEDIA_CONTROLLER)
	max96712->pad.flags = MEDIA_PAD_FL_SOURCE;
	sd->entity.function = MEDIA_ENT_F_CAM_SENSOR;
	ret = media_entity_pads_init(&sd->entity, 1, &max96712->pad);
	if (ret < 0)
		goto err_power_off;
#endif

	memset(facing, 0, sizeof(facing));
	if (strcmp(max96712->module_facing, "back") == 0)
		facing[0] = 'b';
	else
		facing[0] = 'f';

	v4l2_set_subdevdata(sd, max96712);

	snprintf(sd->name, sizeof(sd->name), "m%02d_%s_%s %s",
		 max96712->module_index, facing, MAX96712_NAME,
		 dev_name(sd->dev));
	ret = v4l2_async_register_subdev_sensor(sd);
	if (ret) {
		dev_err(dev, "v4l2 async register subdev failed\n");
		goto err_clean_entity;
	}

	if (!IS_ERR(max96712->lock_gpio)) {
		max96712->hot_plug_irq = gpiod_to_irq(max96712->lock_gpio);
		if (max96712->hot_plug_irq < 0)
			dev_err(dev, "failed to get hot plug irq\n");

		ret = devm_request_threaded_irq(dev,
				max96712->hot_plug_irq,
				NULL,
				max96712_hot_plug_detect_irq_handler,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING | IRQF_ONESHOT,
				"max96712_hot_plug",
				max96712);
		if (ret)
			dev_err(dev, "failed to request hot plug irq (%d)\n", ret);
		disable_irq(max96712->hot_plug_irq);
	}

	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);
	pm_runtime_idle(dev);

	return 0;

err_clean_entity:
#if defined(CONFIG_MEDIA_CONTROLLER)
	media_entity_cleanup(&sd->entity);
#endif
err_power_off:
	__max96712_power_off(max96712);
err_free_handler:
	v4l2_ctrl_handler_free(&max96712->ctrl_handler);
err_destroy_mutex:
	mutex_destroy(&max96712->mutex);

	return ret;
}

static void max96712_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct max96712 *max96712 = v4l2_get_subdevdata(sd);

	v4l2_async_unregister_subdev(sd);
#if defined(CONFIG_MEDIA_CONTROLLER)
	media_entity_cleanup(&sd->entity);
#endif
	v4l2_ctrl_handler_free(&max96712->ctrl_handler);
	mutex_destroy(&max96712->mutex);

	pm_runtime_disable(&client->dev);
	if (!pm_runtime_status_suspended(&client->dev))
		__max96712_power_off(max96712);
	pm_runtime_set_suspended(&client->dev);
}

#if IS_ENABLED(CONFIG_OF)
static const struct of_device_id max96712_of_match[] = {
	{ .compatible = "maxim,max96712" },
	{},
};
MODULE_DEVICE_TABLE(of, max96712_of_match);
#endif

static const struct i2c_device_id max96712_match_id[] = {
	{ "maxim,max96712", 0 },
	{},
};

static struct i2c_driver max96712_i2c_driver = {
	.driver = {
		.name = MAX96712_NAME,
		.pm = &max96712_pm_ops,
		.of_match_table = of_match_ptr(max96712_of_match),
	},
	.probe		= &max96712_probe,
	.remove		= &max96712_remove,
	.id_table	= max96712_match_id,
};

static int __init sensor_mod_init(void)
{
	return i2c_add_driver(&max96712_i2c_driver);
}

static void __exit sensor_mod_exit(void)
{
	i2c_del_driver(&max96712_i2c_driver);
}

module_init(sensor_mod_init);
module_exit(sensor_mod_exit);

MODULE_DESCRIPTION("Maxim max96712 deserializer driver");
MODULE_LICENSE("GPL");
