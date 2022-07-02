/*
 * Copyright (c) 2022 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usb_ch9.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>

// register root level logger
LOG_MODULE_REGISTER(usb_driver, CONFIG_LOG_DEFAULT_LEVEL);
static const char c = ' ';
void main(void)
{
	const struct device *dev;
	uint32_t dtr = 0U;
	int ret;

	dev = DEVICE_DT_GET_ONE(zephyr_cdc_acm_uart);
	if (!device_is_ready(dev))
	{
		LOG_ERR("%cCDC ACM device not ready", c);
		return;
	}
	LOG_INF("%cCDC ACM device is ready", c);

	ret = usb_enable(NULL);
	if (ret < 0)
	{
		LOG_ERR("%cusb_enable() failed", c);
		return;
	}
	LOG_INF("%cusb device enabled", c);

	LOG_INF("%cWait for DTR", c);

	while (true)
	{
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
		if (dtr)
		{
			LOG_INF("%cReceived DTR", c);
			break;
		}
		else
		{
			/* Give CPU resources to low priority threads. */
			k_sleep(K_MSEC(100));
		}
	}
}
