/*
 * Copyright (c) 2022 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/zephyr.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/logging/log.h>

// register root level logger
LOG_MODULE_REGISTER(usb_driver, CONFIG_LOG_DEFAULT_LEVEL);
static const char c = ' ';
#define LED DT_ALIAS(csled)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED, gpios);

#define RING_BUF_SIZE 1024
uint8_t ring_buffer[RING_BUF_SIZE];

struct ring_buf ringbuf;

void ro_usb_dc_status_callback(enum usb_dc_status_code cb_status,
							   const uint8_t *param);
void ro_uart_irq_callback_user_data_t(const struct device *dev,
									  void *user_data);

static void interrupt_handler(const struct device *dev, void *user_data)
{
	int ret;
	ARG_UNUSED(user_data);

	while (uart_irq_update(dev) && uart_irq_is_pending(dev))
	{
		if (uart_irq_rx_ready(dev))
		{
			int recv_len, rb_len;
			uint8_t buffer[64];
			size_t len = MIN(ring_buf_space_get(&ringbuf),
							 sizeof(buffer));

			recv_len = uart_fifo_read(dev, buffer, len);
			if (recv_len < 0)
			{
				recv_len = 0;
			};
			LOG_DBG("Received %d bytes", recv_len);
			if (buffer[0] == '1')
			{
				ret = gpio_pin_set_dt(&led, 1);
				if (ret < 0)
				{
					LOG_ERR("COuld not set led to %d", 1);
					return;
				}
				LOG_DBG("%#x -> Turning on led", buffer[0]);
			}
			else if (buffer[0] == '0')
			{
				ret = gpio_pin_set_dt(&led, 0);
				if (ret < 0)
				{
					LOG_ERR("COuld not set led to %d", 0);
					return;
				}
				LOG_DBG("%#x -> Turning off led", buffer[0]);
			}
			else
			{
				LOG_DBG("%#x -> Unknown command", buffer[0]);
			}
			buffer[1] = '\r';
			buffer[1] = '\n';

			rb_len = ring_buf_put(&ringbuf, buffer, recv_len + 2);
			if (rb_len < recv_len)
			{
				LOG_ERR("Drop %u bytes", recv_len - rb_len);
			}

			LOG_DBG("tty fifo -> ringbuf %d bytes", rb_len);
			if (rb_len)
			{
				uart_irq_tx_enable(dev);
			}
		}

		if (uart_irq_tx_ready(dev))
		{
			uint8_t buffer[64];
			int rb_len, send_len;

			rb_len = ring_buf_get(&ringbuf, buffer, sizeof(buffer));
			if (!rb_len)
			{
				LOG_DBG("Ring buffer empty, disable TX IRQ");
				uart_irq_tx_disable(dev);
				continue;
			}

			send_len = uart_fifo_fill(dev, buffer, rb_len);
			if (send_len < rb_len)
			{
				LOG_ERR("Drop %d bytes", rb_len - send_len);
			}

			LOG_DBG("ringbuf -> tty fifo %d bytes", send_len);
		}
	}
}

void start_transmit(const struct device *dev)
{

	int len, ret;
	char *msg = "Reading of microcontroller\r\n";
	len = strlen(msg);

	while (1)
	{

		ret = uart_fifo_fill(dev, msg, len);
		if (ret < 0)
		{
			LOG_ERR("Error sending msg to uart %d -> %d", len, ret);
		}

		k_sleep(K_SECONDS(1));
	}
}

void main(void)
{

	const struct device *dev;
	uint32_t baudrate, dtr = 0U;
	int ret;

	dev = DEVICE_DT_GET_ONE(zephyr_cdc_acm_uart);
	if (!device_is_ready(dev))
	{
		LOG_ERR("CDC ACM device not ready");
		return;
	}

	ret = usb_enable(NULL);
	if (ret != 0)
	{
		LOG_ERR("Failed to enable USB");
		return;
	}

	ring_buf_init(&ringbuf, sizeof(ring_buffer), ring_buffer);

	LOG_INF("Wait for DTR");

	while (true)
	{
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
		if (dtr)
		{
			break;
		}
		else
		{
			/* Give CPU resources to low priority threads. */
			k_sleep(K_MSEC(100));
		}
	}

	LOG_INF("DTR set");
	start_transmit(dev);

	if (!device_is_ready(led.port))
	{
		LOG_ERR("LED is not ready");
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		LOG_ERR("Could not configure led as output");
		return;
	}

	LOG_DBG("LED is set as output");

	uint8_t count = 10;
	while (count--)
	{
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0)
		{
			return;
		}

		k_msleep(1000);
	}

	/* They are optional, we use them to test the interrupt endpoint */
	ret = uart_line_ctrl_set(dev, UART_LINE_CTRL_DCD, 1);
	if (ret)
	{
		LOG_WRN("Failed to set DCD, ret code %d", ret);
	}

	ret = uart_line_ctrl_set(dev, UART_LINE_CTRL_DSR, 1);
	if (ret)
	{
		LOG_WRN("Failed to set DSR, ret code %d", ret);
	}

	/* Wait 1 sec for the host to do all settings */
	k_busy_wait(1000000);

	ret = uart_line_ctrl_get(dev, UART_LINE_CTRL_BAUD_RATE, &baudrate);
	if (ret)
	{
		LOG_WRN("Failed to get baudrate, ret code %d", ret);
	}
	else
	{
		LOG_INF("Baudrate detected: %d", baudrate);
	}

	uart_irq_callback_set(dev, interrupt_handler);

	/* Enable rx interrupts */
	uart_irq_rx_enable(dev);

	// start_transmit(dev);
}

/**
 * @brief
 *
 * @param cb_status
 * @param param
 */
void ro_usb_dc_status_callback(enum usb_dc_status_code cb_status, const uint8_t *param)
{
	char *status = NULL;
	switch (cb_status)
	{
	case USB_DC_ERROR:
		status = "USB_DC_ERROR";
		break;
	case USB_DC_RESET:
		status = "USB_DC_RESET";
		break;
	case USB_DC_CONNECTED:
		status = "USB_DC_CONNECTED";
		break;
	case USB_DC_DISCONNECTED:
		status = "USB_DC_DISCONNECTED";
		break;
	case USB_DC_CONFIGURED:
		status = "USB_DC_CONFIGURED";
		break;
	case USB_DC_SUSPEND:
		status = "USB_DC_SUSPEND";
		break;
	case USB_DC_RESUME:
		status = "USB_DC_RESUME";
		break;
	case USB_DC_INTERFACE:
		status = "USB_DC_INTERFACE";
		break;
	case USB_DC_CLEAR_HALT:
		status = "USB_DC_CLEAR_HALT";
		break;
	case USB_DC_SOF:
		status = "USB_DC_SOF";
		break;
	case USB_DC_UNKNOWN:
		status = "USB_DC_UNKNOWN";
		break;
	case USB_DC_SET_HALT:
		status = "USB_DC_ERROR";
		break;
	}
	LOG_INF("%cro_usb_dc_status_callback(): param=%hu, status=%s", c, (unsigned short)(*param), status);
}

void ro_uart_irq_callback_user_data_t(const struct device *dev, void *user_data)
{
	ARG_UNUSED(user_data);

	while (uart_irq_update(dev) && uart_irq_is_pending(dev))
	{
		if (uart_irq_rx_ready(dev))
		{
			LOG_INF("ro_uart_irq_callback_user_data_t(): RX is ready");
		}
		if (uart_irq_tx_ready(dev))
		{
			LOG_INF("ro_uart_irq_callback_user_data_t(): TX is ready");
		}
	}
}