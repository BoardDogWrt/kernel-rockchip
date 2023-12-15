// SPDX-License-Identifier: GPL-2.0+
/*
 * rockchip SerDes Panele driver for drm platform
 *
 * Copyright (C) 2022 Rockchip Electronics Co. Ltd.
 */

#include <linux/backlight.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/of_graph.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio/consumer.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>

#include <video/display_timing.h>
#include <video/of_display_timing.h>
#include <video/videomode.h>

#include <drm/drm_crtc.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>

#include "rkx110_x120.h"
#include "rkx120_dsi_tx.h"

static inline struct rk_serdes_panel *to_serdes_panel(struct drm_panel *panel)
{
	return container_of(panel, struct rk_serdes_panel, panel);
}

static int serdes_panel_hw_prepare(struct rk_serdes_panel *sd_panel)
{
	if (sd_panel->supply) {
		int err;

		err = regulator_enable(sd_panel->supply);
		if (err < 0) {
			dev_err(sd_panel->dev, "failed to enable supply: %d\n",
				err);
			return err;
		}
		mdelay(20);
	}

	if (sd_panel->enable_gpio) {
		gpiod_set_value_cansleep(sd_panel->enable_gpio, 1);
		mdelay(20);
	}

	if (sd_panel->reset_gpio) {
		gpiod_set_value_cansleep(sd_panel->reset_gpio, 1);
		mdelay(20);
	}

	return 0;
}

static int serdes_panel_dsi_prepare(struct rk_serdes_panel *sd_panel)
{
	struct rk_serdes *serdes = sd_panel->parent;

	if (sd_panel->id == 0 && sd_panel->route.remote0_port0 == RK_SERDES_DSI_TX0 &&
	    !!(sd_panel->on_cmds))
		rkx120_dsi_tx_cmd_seq_xfer(serdes, &sd_panel->dsi_tx, DEVICE_REMOTE0,
					   sd_panel->on_cmds);

	if (sd_panel->id == 1 && sd_panel->route.remote1_port0 == RK_SERDES_DSI_TX0 &&
	    !!(sd_panel->on_cmds))
		rkx120_dsi_tx_cmd_seq_xfer(serdes, &sd_panel->dsi_tx, DEVICE_REMOTE1,
					   sd_panel->on_cmds);

	return 0;
}

static int serdes_panel_prepare(struct drm_panel *panel)
{
	struct rk_serdes_panel *sd_panel = to_serdes_panel(panel);
	struct rk_serdes *serdes = sd_panel->parent;
	int ret;

	ret = serdes_panel_hw_prepare(sd_panel);
	if (ret)
		return ret;
	if (sd_panel->secondary) {
		ret = serdes_panel_hw_prepare(sd_panel->secondary);
		if (ret)
			return ret;
	}

	if (serdes->route_prepare)
		serdes->route_prepare(serdes, &sd_panel->route);

	serdes_panel_dsi_prepare(sd_panel);
	if (sd_panel->secondary)
		serdes_panel_dsi_prepare(sd_panel->secondary);

	return 0;
}

static int serdes_panel_enable(struct drm_panel *panel)
{
	struct rk_serdes_panel *sd_panel = to_serdes_panel(panel);
	struct rk_serdes *serdes = sd_panel->parent;
	int ret;

	if (serdes->route_enable)
		serdes->route_enable(serdes, &sd_panel->route);

	if (sd_panel->secondary) {
		ret = backlight_enable(sd_panel->secondary->panel.backlight);
		if (ret < 0) {
			dev_err(sd_panel->dev, "failed to enable backlight: %d\n", ret);
			return ret;
		}
	}

	return 0;
}

static int serdes_panel_disable(struct drm_panel *panel)
{
	struct rk_serdes_panel *sd_panel = to_serdes_panel(panel);
	struct rk_serdes *serdes = sd_panel->parent;
	int ret;

	if (sd_panel->secondary) {
		ret = backlight_disable(sd_panel->secondary->panel.backlight);
		if (ret < 0) {
			dev_err(sd_panel->dev, "failed to disable backlight: %d\n", ret);
			return ret;
		}
	}

	if (serdes->route_disable)
		serdes->route_disable(serdes, &sd_panel->route);

	return 0;
}

static int serdes_panel_hw_unprepare(struct rk_serdes_panel *sd_panel)
{
	if (sd_panel->reset_gpio) {
		gpiod_set_value_cansleep(sd_panel->reset_gpio, 0);
		mdelay(20);
	}

	if (sd_panel->enable_gpio) {
		gpiod_set_value_cansleep(sd_panel->enable_gpio, 0);
		mdelay(20);
	}

	if (sd_panel->supply)
		regulator_disable(sd_panel->supply);

	return 0;
}

static int serdes_panel_dsi_unprepare(struct rk_serdes_panel *sd_panel)
{
	struct rk_serdes *serdes = sd_panel->parent;

	if (sd_panel->id == 0 && sd_panel->route.remote0_port0 == RK_SERDES_DSI_TX0 &&
	    !!(sd_panel->off_cmds))
		rkx120_dsi_tx_cmd_seq_xfer(serdes, &sd_panel->dsi_tx, DEVICE_REMOTE0,
					   sd_panel->off_cmds);

	if (sd_panel->id == 1 && sd_panel->route.remote1_port0 == RK_SERDES_DSI_TX0 &&
	    !!(sd_panel->off_cmds))
		rkx120_dsi_tx_cmd_seq_xfer(serdes, &sd_panel->dsi_tx, DEVICE_REMOTE1,
					   sd_panel->off_cmds);

	return 0;
}

static int serdes_panel_unprepare(struct drm_panel *panel)
{
	struct rk_serdes_panel *sd_panel = to_serdes_panel(panel);
	struct rk_serdes *serdes = sd_panel->parent;

	serdes_panel_dsi_unprepare(sd_panel);
	if (sd_panel->secondary)
		serdes_panel_dsi_unprepare(sd_panel->secondary);

	if (serdes->route_unprepare)
		serdes->route_unprepare(serdes, &sd_panel->route);

	serdes_panel_hw_unprepare(sd_panel);
	if (sd_panel->secondary)
		serdes_panel_hw_unprepare(sd_panel->secondary);

	return 0;
}

static int serdes_panel_of_get_native_mode(struct rk_serdes_panel *sd_panel,
					   struct drm_connector *connector)
{
	struct rk_serdes_route *route = &sd_panel->route;
	struct device_node *timings_np;
	struct drm_display_mode *mode;
	struct drm_device *drm = connector->dev;
	u32 bus_flags;
	int ret;

	timings_np = of_get_child_by_name(sd_panel->dev->of_node, "display-timings");
	if (!timings_np) {
		dev_err(sd_panel->dev, "failed to find display-timings node\n");
		return 0;
	}

	of_node_put(timings_np);

	mode = drm_mode_create(drm);
	if (!mode)
		return 0;

	ret = of_get_drm_display_mode(sd_panel->dev->of_node, mode,
				      &bus_flags, OF_USE_NATIVE_MODE);
	if (ret) {
		dev_err(sd_panel->dev, "failed to find dts display timings\n");
		drm_mode_destroy(drm, mode);
		return 0;
	}

	drm_mode_set_name(mode);
	connector->display_info.bus_flags = bus_flags;
	mode->type |= DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	drm_mode_probed_add(connector, mode);
	drm_display_mode_to_videomode(mode, &sd_panel->route.vm);

	if (route->frame_mode == SERDES_SP_LEFT_RIGHT_SPLIT ||
	    route->frame_mode == SERDES_SP_PIXEL_INTERLEAVED) {
		sd_panel->route.vm.pixelclock /= 2;
		sd_panel->route.vm.hactive /= 2;
		sd_panel->route.vm.hfront_porch /= 2;
		sd_panel->route.vm.hback_porch /= 2;
		sd_panel->route.vm.hsync_len /= 2;
	} else if (route->frame_mode == SERDES_SP_LINE_INTERLEAVED) {
		sd_panel->route.vm.pixelclock /= 2;
		sd_panel->route.vm.vactive /= 2;
		sd_panel->route.vm.vfront_porch /= 2;
		sd_panel->route.vm.vback_porch /= 2;
		sd_panel->route.vm.vsync_len /= 2;
	}

	return 1;
}

static int serdes_panel_get_modes(struct drm_panel *panel,
				struct drm_connector *connector)
{
	struct rk_serdes_panel *sd_panel = to_serdes_panel(panel);
	int num = 0;

	num += serdes_panel_of_get_native_mode(sd_panel, connector);
	drm_display_info_set_bus_formats(&connector->display_info,
					 &sd_panel->bus_format, 1);

	return num;
}

static const struct drm_panel_funcs serdes_panel_funcs = {
	.prepare = serdes_panel_prepare,
	.enable = serdes_panel_enable,
	.disable = serdes_panel_disable,
	.unprepare = serdes_panel_unprepare,
	.get_modes = serdes_panel_get_modes,
};

static int
dsi_panel_parse_cmds(struct device *dev, const u8 *data,
		     int blen, struct panel_cmds *pcmds)
{
	unsigned int len;
	char *buf, *bp;
	struct cmd_ctrl_hdr *dchdr;
	int i, cnt;

	if (!pcmds)
		return -EINVAL;

	buf = devm_kmemdup(dev, data, blen, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	/* scan init commands */
	bp = buf;
	len = blen;
	cnt = 0;
	while (len > sizeof(*dchdr)) {
		dchdr = (struct cmd_ctrl_hdr *)bp;

		if (dchdr->dlen > len) {
			dev_err(dev, "%s: error, len=%d", __func__, dchdr->dlen);
			return -EINVAL;
		}

		bp += sizeof(*dchdr);
		len -= sizeof(*dchdr);
		bp += dchdr->dlen;
		len -= dchdr->dlen;
		cnt++;
	}

	if (len != 0) {
		dev_err(dev, "%s: dcs_cmd=%x len=%d error!", __func__, buf[0], blen);
		return -EINVAL;
	}

	pcmds->cmds = devm_kcalloc(dev, cnt, sizeof(struct cmd_desc), GFP_KERNEL);
	if (!pcmds->cmds)
		return -ENOMEM;

	pcmds->cmd_cnt = cnt;
	pcmds->buf = buf;
	pcmds->blen = blen;

	bp = buf;
	len = blen;
	for (i = 0; i < cnt; i++) {
		dchdr = (struct cmd_ctrl_hdr *)bp;
		len -= sizeof(*dchdr);
		bp += sizeof(*dchdr);
		pcmds->cmds[i].dchdr = *dchdr;
		pcmds->cmds[i].payload = bp;
		bp += dchdr->dlen;
		len -= dchdr->dlen;
	}

	return 0;
}

static int serdes_dsi_panel_get_cmds(struct rk_serdes_panel *sd_panel)
{
	struct device_node *np = sd_panel->dev->of_node;
	struct device *dev = sd_panel->dev;
	const void *data;
	int len, err;

	data = of_get_property(np, "panel-init-sequence", &len);
	if (data) {
		sd_panel->on_cmds = devm_kzalloc(dev, sizeof(*sd_panel->on_cmds), GFP_KERNEL);
		if (!sd_panel->on_cmds)
			return -ENOMEM;

		err = dsi_panel_parse_cmds(dev, data, len, sd_panel->on_cmds);
		if (err) {
			dev_err(dev, "failed to parse dsi panel init sequence\n");
			return err;
		}
	}

	data = of_get_property(np, "panel-exit-sequence", &len);
	if (data) {
		sd_panel->off_cmds = devm_kzalloc(dev, sizeof(*sd_panel->off_cmds), GFP_KERNEL);
		if (!sd_panel->off_cmds)
			return -ENOMEM;

		err = dsi_panel_parse_cmds(dev, data, len, sd_panel->off_cmds);
		if (err) {
			dev_err(dev, "failed to parse dsi panel exit sequence\n");
			return err;
		}
	}

	return 0;
}

static struct mipi_dsi_device *serdes_attach_dsi(struct rk_serdes_panel *sd_panel,
						 struct device_node *dsi_node)
{
	const struct mipi_dsi_device_info info = { "serdes", 0, NULL };
	struct mipi_dsi_device *dsi;
	struct mipi_dsi_host *host;
	int ret;

	host = of_find_mipi_dsi_host_by_node(dsi_node);
	if (!host) {
		dev_err(sd_panel->dev, "failed to find dsi host\n");
		return ERR_PTR(-EPROBE_DEFER);
	}

	dsi = mipi_dsi_device_register_full(host, &info);
	if (IS_ERR(dsi)) {
		dev_err(sd_panel->dev, "failed to create dsi device\n");
		return dsi;
	}

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_LPM | MIPI_DSI_MODE_NO_EOT_PACKET |
			  MIPI_DSI_CLOCK_NON_CONTINUOUS;

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		dev_err(sd_panel->dev, "failed to attach dsi to host\n");
		mipi_dsi_device_unregister(dsi);
		return ERR_PTR(ret);
	}

	return dsi;
}

static int rkx110_dsi_rx_parse(struct rk_serdes_panel *sd_panel)
{
	struct device_node *np = sd_panel->dev->of_node;
	struct rkx110_dsi_rx *dsi_rx = &sd_panel->dsi_rx;
	struct mipi_dsi_device *dsi;
	struct device_node *dsi_node;
	u32 val;

	if (of_property_read_u32(np, "dsi-rx,lanes", &val))
		dsi_rx->lanes = 4;
	else
		dsi_rx->lanes = val;

	if (of_property_read_bool(np, "dsi-rx,video-mode"))
		dsi_rx->mode_flags |= SERDES_MIPI_DSI_MODE_LPM | SERDES_MIPI_DSI_MODE_VIDEO |
				      SERDES_MIPI_DSI_MODE_VIDEO_BURST;
	else
		dsi_rx->mode_flags |= SERDES_MIPI_DSI_MODE_LPM;

	dsi_node = of_graph_get_remote_node(np, 0, -1);
	if (!dsi_node)
		dev_err(sd_panel->dev, "failed to get remote node for primary dsi\n");

	dsi = serdes_attach_dsi(sd_panel, dsi_node);
	if (IS_ERR(dsi))
		return -EPROBE_DEFER;

	return 0;
}

static int rkx120_dsi_tx_parse(struct rk_serdes_panel *sd_panel)
{
	struct device_node *np = sd_panel->dev->of_node;
	struct rkx120_dsi_tx *dsi_tx = &sd_panel->dsi_tx;
	const char *string;
	int ret;
	u32 val;

	dsi_tx->combtxphy = &sd_panel->combtxphy;

	if (of_property_read_u32(np, "dsi-tx,lanes", &val))
		dsi_tx->lanes = 4;
	else
		dsi_tx->lanes = val;

	if (of_property_read_bool(np, "dsi-tx,video-mode"))
		dsi_tx->mode_flags |= SERDES_MIPI_DSI_MODE_LPM | SERDES_MIPI_DSI_MODE_VIDEO |
				      SERDES_MIPI_DSI_MODE_VIDEO_BURST;
	else
		dsi_tx->mode_flags |= SERDES_MIPI_DSI_MODE_LPM;

	if (of_property_read_bool(np, "dsi-tx,eotp"))
		dsi_tx->mode_flags |= SERDES_MIPI_DSI_MODE_EOT_PACKET;

	if (!of_property_read_string(np, "dsi-tx,format", &string)) {
		if (!strcmp(string, "rgb666")) {
			dsi_tx->bus_format = SERDES_MIPI_DSI_FMT_RGB666;
			dsi_tx->bpp = 24;
		} else if (!strcmp(string, "rgb666-packed")) {
			dsi_tx->bus_format = SERDES_MIPI_DSI_FMT_RGB666_PACKED;
			dsi_tx->bpp = 18;
		} else if (!strcmp(string, "rgb565")) {
			dsi_tx->bus_format = SERDES_MIPI_DSI_FMT_RGB565;
			dsi_tx->bpp = 16;
		} else {
			dsi_tx->bus_format = SERDES_MIPI_DSI_FMT_RGB888;
			dsi_tx->bpp = 24;
		}
	} else {
		dsi_tx->bus_format = SERDES_MIPI_DSI_FMT_RGB888;
		dsi_tx->bpp = 24;
	}

	ret = serdes_dsi_panel_get_cmds(sd_panel);
	if (ret) {
		dev_err(sd_panel->dev, "failed to get cmds\n");
		return ret;
	}

	return 0;
}

static int serdes_panel_parse_route(struct rk_serdes_panel *sd_panel)
{
	struct rk_serdes_route *route = &sd_panel->route;

	device_property_read_u32(sd_panel->dev, "local-port0", &route->local_port0);
	device_property_read_u32(sd_panel->dev, "local-port1", &route->local_port1);
	device_property_read_u32(sd_panel->dev, "remote0-port0", &route->remote0_port0);
	device_property_read_u32(sd_panel->dev, "remote0-port1", &route->remote0_port1);
	device_property_read_u32(sd_panel->dev, "remote1-port0", &route->remote1_port0);
	device_property_read_u32(sd_panel->dev, "remote1-port1", &route->remote1_port1);

	if (!route->local_port0 && !route->local_port1) {
		dev_err(sd_panel->dev, "local port should set\n");
		return -EINVAL;
	}

	if (route->local_port0 && !route->remote0_port0) {
		dev_err(sd_panel->dev, "remote0_port0 should set\n");
		return -EINVAL;
	}

	if (route->local_port1 && !route->remote1_port0) {
		dev_err(sd_panel->dev, "remote1_port0 should set\n");
		return -EINVAL;
	}

	if (route->remote1_port0 && route->remote0_port1) {
		dev_err(sd_panel->dev, "too many output\n");
		return -EINVAL;
	}

	route->frame_mode = SERDES_FRAME_NORMAL_MODE;
	route->route_flag = 0;

	/* 2 video stream output in a route */
	if (route->local_port0 && (route->remote1_port0 || route->remote0_port1)) {
		if (route->remote1_port0)
			route->route_flag |= ROUTE_MULTI_REMOTE | ROUTE_MULTI_CHANNEL|
					     ROUTE_MULTI_LANE;

		if (route->remote0_port1) {
			if ((route->remote0_port0 == RK_SERDES_LVDS_TX0) &&
			    (route->remote0_port1 == RK_SERDES_LVDS_TX1)) {
				route->route_flag |= ROUTE_MULTI_CHANNEL;
			} else if ((route->remote0_port0 == RK_SERDES_LVDS_TX1) &&
				    (route->remote0_port1 == RK_SERDES_LVDS_TX0)) {
				route->route_flag |= ROUTE_MULTI_CHANNEL;
			} else {
				dev_err(sd_panel->dev, "invalid multi output type\n");
				return -EINVAL;
			}
		}

		if (route->local_port0) {
			if (device_property_read_bool(sd_panel->dev, "split-mode")) {
				/* only dsi input support split mode */
				if ((route->local_port0 != RK_SERDES_DSI_RX0) &&
				    (route->local_port0 != RK_SERDES_DSI_RX1)) {
					dev_err(sd_panel->dev,
						"local_port should be dsi in split mode\n");
					return -EINVAL;
				}
				if (device_property_read_bool(sd_panel->dev,
							      "sf-pixel-interleaved"))
					route->frame_mode = SERDES_SP_PIXEL_INTERLEAVED;
				else if (device_property_read_bool(sd_panel->dev,
								   "sf-line-interleaved"))
					route->frame_mode = SERDES_SP_LINE_INTERLEAVED;
				else
					route->frame_mode = SERDES_SP_LEFT_RIGHT_SPLIT;

				route->route_flag |= ROUTE_MULTI_SPLIT;

			} else  {
				route->route_flag |= ROUTE_MULTI_MIRROR;
			}
		}
	}

	return 0;
}

static int serdes_panel_match_by_id(struct device *dev, const void *data)
{
	struct rk_serdes_panel *sd_panel = dev_get_drvdata(dev);
	unsigned int *id = (unsigned int *)data;

	return sd_panel->id == *id;
}

static struct rk_serdes_panel *serdes_panel_find_by_id(struct device_driver *drv,
						       unsigned int id)
{
	struct device *dev;

	dev = driver_find_device(drv, NULL, &id, serdes_panel_match_by_id);
	if (!dev)
		return NULL;

	return dev_get_drvdata(dev);
}

static int serdes_panel_probe(struct platform_device *pdev)
{
	struct rk_serdes *serdes = dev_get_drvdata(pdev->dev.parent);
	struct rk_serdes_panel *sd_panel, *secondary;
	int ret;
	u32 reg;

	sd_panel = devm_kzalloc(&pdev->dev, sizeof(*sd_panel), GFP_KERNEL);
	if (!sd_panel)
		return -ENOMEM;

	sd_panel->dev = &pdev->dev;
	sd_panel->parent = serdes;
	sd_panel->route.stream_type = STREAM_DISPLAY;

	ret = of_property_read_u32(sd_panel->dev->of_node, "reg", &reg);
	if (ret)
		sd_panel->id = 0;
	sd_panel->id = reg;

	sd_panel->multi_panel = device_property_read_bool(sd_panel->dev, "multi-panel");
	if (sd_panel->multi_panel) {
		secondary = serdes_panel_find_by_id(sd_panel->dev->driver, 1);
		if (!secondary)
			return -EPROBE_DEFER;
		sd_panel->secondary = secondary;
		dev_info(sd_panel->dev, "%s get secondary panel\n", __func__);
	}

	sd_panel->supply = devm_regulator_get_optional(sd_panel->dev, "power");
	if (IS_ERR(sd_panel->supply)) {
		ret = PTR_ERR(sd_panel->supply);

		if (ret != -ENODEV) {
			if (ret != -EPROBE_DEFER)
				dev_err(sd_panel->dev, "failed to request regulator: %d\n",
					ret);
			return ret;
		}

		sd_panel->supply = NULL;
	}

	/* Get GPIOs and backlight controller. */
	sd_panel->enable_gpio = devm_gpiod_get_optional(sd_panel->dev, "enable",
							GPIOD_OUT_LOW);
	if (IS_ERR(sd_panel->enable_gpio)) {
		ret = PTR_ERR(sd_panel->enable_gpio);
		dev_err(sd_panel->dev, "failed to request %s GPIO: %d\n",
			"enable", ret);
		return ret;
	}

	sd_panel->reset_gpio = devm_gpiod_get_optional(sd_panel->dev, "reset",
						       GPIOD_OUT_HIGH);
	if (IS_ERR(sd_panel->reset_gpio)) {
		ret = PTR_ERR(sd_panel->reset_gpio);
		dev_err(sd_panel->dev, "failed to request %s GPIO: %d\n",
			"reset", ret);
		return ret;
	}

	ret = serdes_panel_parse_route(sd_panel);
	if (ret < 0)
		return ret;

	if (sd_panel->route.remote0_port0 & RK_SERDES_DSI_TX0 ||
		sd_panel->route.remote1_port0 & RK_SERDES_DSI_TX0) {
		ret = rkx120_dsi_tx_parse(sd_panel);
		if (ret) {
			dev_err(sd_panel->dev, "failed to get cmds\n");
			return ret;
		}
	}

	/* Register the panel. */
	drm_panel_init(&sd_panel->panel, sd_panel->dev, &serdes_panel_funcs, 0);

	ret = drm_panel_of_backlight(&sd_panel->panel);
	if (ret)
		return ret;

	drm_panel_add(&sd_panel->panel);

	if ((sd_panel->route.local_port0 & RK_SERDES_DSI_RX0 ||
	    sd_panel->route.local_port0 & RK_SERDES_DSI_RX1) && sd_panel->id == 0) {
		ret = rkx110_dsi_rx_parse(sd_panel);
		if (ret < 0) {
			drm_panel_remove(&sd_panel->panel);
			return ret;
		}
	}

	if ((sd_panel->route.local_port1 & RK_SERDES_DSI_RX0 ||
		sd_panel->route.local_port1 & RK_SERDES_DSI_RX1) && sd_panel->id == 1) {
		ret = rkx110_dsi_rx_parse(sd_panel);
		if (ret < 0) {
			drm_panel_remove(&sd_panel->panel);
			return ret;
		}
	}

	dev_set_drvdata(sd_panel->dev, sd_panel);

	if (sd_panel->route.route_flag & ROUTE_MULTI_CHANNEL)
		serdes->channel_nr = 2;

	if (sd_panel->route.local_port0 && sd_panel->id == 0)
		serdes->route_nr++;

	if (sd_panel->route.local_port1 && sd_panel->id == 1)
		serdes->route_nr++;

	if (serdes->route_nr == 2)
		serdes->channel_nr = 2;

	return 0;
}

static int serdes_panel_remove(struct platform_device *pdev)
{
	struct rk_serdes_panel *sd_panel = dev_get_drvdata(&pdev->dev);

	drm_panel_remove(&sd_panel->panel);

	drm_panel_disable(&sd_panel->panel);

	return 0;
}

static const struct of_device_id serdes_panel_of_table[] = {
	{ .compatible = "rockchip,serdes-panel", },
	{ /* Sentinel */ },
};

MODULE_DEVICE_TABLE(of, serdes_panel_of_table);

static struct platform_driver serdes_panel_driver = {
	.probe		= serdes_panel_probe,
	.remove		= serdes_panel_remove,
	.driver		= {
		.name	= "rockchip-serdes-panel",
		.of_match_table = serdes_panel_of_table,
	},
};

module_platform_driver(serdes_panel_driver);

MODULE_AUTHOR("Zhang Yubing <yubing.zhang@rock-chips.com>");
MODULE_DESCRIPTION("RKx110 RKx120 Panel Driver");
MODULE_LICENSE("GPL");
