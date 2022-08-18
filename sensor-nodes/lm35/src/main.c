/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usb_ch9.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>
#include "../usb-frame.h"

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7
#define MAX_PACKET USB_MAX_FS_BULK_MPS
#define RO_EP_OUT_ADDR 0X01
#define RO_EP_IN_ADDR 0X81

LOG_MODULE_REGISTER(blue_pill_lm35, LOG_LEVEL_DBG);

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static struct adc_dt_spec lm35 = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);

void blink(void *, void *);
void handle_lm35();
void work_handler();
void handle_led(uint8_t cmd, const struct gpio_dt_spec *p_spec);
void callback_ep_out(uint8_t ep, enum usb_dc_ep_cb_status_code cb_status);
void callback_ep_in(uint8_t ep, enum usb_dc_ep_cb_status_code cb_status);
void led_setup(const struct gpio_dt_spec *spec);
void ro_cb_usb_status(struct usb_cfg_data *cfg,
					  enum usb_dc_status_code cb_status,
					  const uint8_t *param);
struct ro_usb_config
{
	struct usb_if_descriptor if0;
	struct usb_ep_descriptor if0_out_ep;
	struct usb_ep_descriptor if0_in_ep;
} __packed;

USBD_CLASS_DESCR_DEFINE(primary, 0)
struct ro_usb_config ro_cfg = {
	// interface descriptor
	.if0 = {
		.bLength = sizeof(struct usb_if_descriptor),
		.bDescriptorType = USB_DESC_INTERFACE,
		.bInterfaceNumber = 0,
		.bAlternateSetting = 0,
		.bNumEndpoints = 2,
		.bInterfaceClass = USB_BCC_VENDOR, // 255
		.bInterfaceSubClass = 0,
		.bInterfaceProtocol = 0,
		.iInterface = 0},

	// data endpoint out
	.if0_out_ep = {.bLength = sizeof(struct usb_ep_descriptor), .bDescriptorType = USB_DESC_ENDPOINT, .bEndpointAddress = RO_EP_OUT_ADDR, .bInterval = 0x00, .wMaxPacketSize = 0x0040, .bmAttributes = USB_DC_EP_BULK},
	.if0_in_ep = {.bLength = sizeof(struct usb_ep_descriptor), .bDescriptorType = USB_DESC_ENDPOINT, .bEndpointAddress = RO_EP_IN_ADDR, .bInterval = 0x00, .wMaxPacketSize = 0x0040, .bmAttributes = USB_DC_EP_BULK}

};

/**
 * @brief Pointer to array of individual endpoint configurations in entire device descriptors
 * 	- used in usb_cfg_data
 */
static struct usb_ep_cfg_data ep_cfg[] = {
	{.ep_cb = callback_ep_out,
	 .ep_addr = RO_EP_OUT_ADDR},
	{.ep_cb = callback_ep_in,
	 .ep_addr = RO_EP_IN_ADDR}};

USBD_CFG_DATA_DEFINE(primary, rohit)
struct usb_cfg_data ro_usb_config = {
	.usb_device_description = NULL,		 // pointer to ch9 usb device descriptor
	.interface_descriptor = &ro_cfg.if0, // pointer to ch9 interface descriptor (usb_if_descriptor)
	.num_endpoints = ARRAY_SIZE(ep_cfg), // total number of endpoints in device descriptor table
	.endpoint = ep_cfg,					 // pointer to array of endpoints cgf (usb_ep_cfg_data)
	.cb_usb_status = ro_cb_usb_status	 // usb status change callback function
};

const struct gpio_dt_spec led_builtin = GPIO_DT_SPEC_GET(DT_ALIAS(led_built_in), gpios);
const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(DT_ALIAS(led_red), gpios);
const struct gpio_dt_spec led_yellow = GPIO_DT_SPEC_GET(DT_ALIAS(led_yellow), gpios);
const struct gpio_dt_spec led_white = GPIO_DT_SPEC_GET(DT_ALIAS(led_white), gpios);
const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(DT_ALIAS(led_green), gpios);

/**
 * @brief Blink task to verfy that scheduler is working
 *
 */
K_THREAD_DEFINE(blink0_id, STACKSIZE, blink, &led_builtin, 1000, NULL, PRIORITY, 0, 0);

/* Actual work of processing commands is done by the threads */
K_THREAD_DEFINE(worker_thread_id, STACKSIZE, work_handler, NULL, NULL, NULL, PRIORITY, 0, 0);

/* Define kernel fifo for sending commands to worker thread */
K_FIFO_DEFINE(work_fifo);

// buffer to hold adc raeading
int16_t buf;
struct adc_sequence sequence = {
	.buffer = &buf,
	/* buffer size in bytes, not number of samples */
	.buffer_size = sizeof(buf),
};

void main(void)
{
	led_setup(&led_red);
	led_setup(&led_yellow);
	led_setup(&led_white);
	led_setup(&led_green);

	int ret = usb_enable(NULL);
	if (ret < 0)
	{
		LOG_ERR("usb_enable() failed");
		return;
	}
	int err;

	if (!device_is_ready(lm35.dev))
	{
		LOG_ERR("ADC controller device not ready");
		return;
	}

	err = adc_channel_setup_dt(&lm35);
	if (err < 0)
	{
		LOG_ERR("Could not setup lm35 (%d)", err);
		return;
	}
}

void blink(void *p1, void *p2)
{
	int ret;
	struct gpio_dt_spec *spec = (struct gpio_dt_spec *)p1;
	int sleep_ms = (int)p2;
	if (!device_is_ready(spec->port))
	{
		LOG_ERR("Error: %s device is not ready", spec->port->name);
		return;
	}

	ret = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
	if (ret != 0)
	{
		LOG_ERR("Error %d: failed to configure pin %d",
				ret, spec->pin);
		return;
	}

	while (1)
	{
		gpio_pin_toggle_dt(spec);
		k_msleep(sleep_ms);
	}
}

/**
 * @brief Callback function for usb device status change
 *
 * @param cfg pointer to active configuration
 * @param cb_status usb device status
 * @param param
 */
void ro_cb_usb_status(struct usb_cfg_data *cfg, enum usb_dc_status_code cb_status, const uint8_t *param)
{
	ARG_UNUSED(param); // declare param as unused argument
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
	LOG_INF(" ro_usb_dc_status_callback(): param=%hu, status=%s", (unsigned short)(*param), status);
}

//* functions for endpoint

/**
 * @brief Callback function for OUT endpoint event
 * 	Indicates data is received and available to application
 *
 * @param ep endpoint address
 * @param cb_status setup=0, out=1, in=0
 */
void callback_ep_out(uint8_t ep, enum usb_dc_ep_cb_status_code cb_status)
{
	uint32_t read_bytes;
	int ret;
	// frame_t *frame;
	frame_t *frame = k_malloc(sizeof(frame_t));
	uint8_t data[MAX_PACKET]; // buffer to hold max packet length

	// read from usb buffer
	ret = usb_read(RO_EP_OUT_ADDR, data, 64, &read_bytes);
	if (ret < 0)
	{
		LOG_ERR("usb_read() failed %d", ret);
		// return ret;
		return;
	}

	printk("\n");

	LOG_INF("Read %d bytes", read_bytes);

	memcpy(frame, data, sizeof(frame_t));

	k_fifo_put(&work_fifo, frame);

	// data is read and application can now use it
}

void work_handler()
{
	LOG_DBG("Started work handler thread");
	frame_t *frame;

	while (1)
	{
		frame = k_fifo_get(&work_fifo,
						   K_FOREVER);
		LOG_DBG("peripheral_id: %#x", frame->peripheral_id);
		LOG_INF("cmd: %#x", frame->cmd);
		LOG_INF("payoload_len: %#x", frame->payload_len);

		switch (frame->peripheral_id)
		{
		case LED_BUILTIN:
			handle_led(frame->cmd, &led_builtin);
			break;
		case LED_YELLOW:
			LOG_INF("Yellow led");
			handle_led(frame->cmd, &led_yellow);
			break;
		case LED_RED:
			handle_led(frame->cmd, &led_red);
			break;
		case LED_WHITE:
			handle_led(frame->cmd, &led_white);
			break;
		case LED_GREEN:
			handle_led(frame->cmd, &led_green);
			break;
		case LM35:
			handle_lm35(frame);
		default:
			LOG_INF("Invalid peripheral ");
			break;
		}
	}

	k_free(frame);
}

/**
 * @brief Callback function for IN endpoint event
 * 	Called when data transmit is done
 *
 * @param ep endpoint address
 * @param cb_status setup=0, out=1, in=0
 */
void callback_ep_in(uint8_t ep, enum usb_dc_ep_cb_status_code cb_status)
{
	LOG_DBG("Callback for Endpoint (updated msg): %#x, Status: %#x", ep, cb_status);
}

void handle_led(uint8_t cmd, const struct gpio_dt_spec *p_spec)
{
	switch (cmd)
	{
	case LED_ON:
		gpio_pin_set_dt(p_spec, 0x01);
		break;
	case LED_OFF:
		gpio_pin_set_dt(p_spec, 0x00);
		break;
	case LED_TOGGLE:
		LOG_INF("Toggling");
		gpio_pin_toggle_dt(p_spec);
		break;
	case LED_BLINK:
		gpio_pin_toggle_dt(p_spec);
		k_msleep(500);
		gpio_pin_toggle_dt(p_spec);
		break;
	default:
		break;
	}
}

void handle_lm35(frame_t *frame)
{
	int err, ret;
	uint32_t read_bytes;
	int32_t val_mv;

	LOG_INF("ADC reading: ");

	(void)adc_sequence_init_dt(&lm35, &sequence);

	err = adc_read(lm35.dev, &sequence);
	if (err < 0)
	{
		LOG_ERR("Could not read (%d)", err);
	}
	else
	{
		LOG_INF("16 bit value %#x", buf);
	}

	/* conversion to mV may not be supported, skip if not */
	val_mv = buf;
	err = adc_raw_to_millivolts_dt(&lm35,
								   &val_mv);
	if (err < 0)
	{
		LOG_ERR(" (value in mV not available)");
	}
	else
	{
		LOG_INF("millivolts: %#x && %hd", val_mv, (int16_t)val_mv);
	}

	frame_ret_t * buff = (frame_ret_t *) k_malloc(sizeof(frame_ret_t));
	buff->cmd = frame->cmd;
	buff->peripheral_id = frame->peripheral_id;
	buff->payload_len = 2;
	buff->value = (uint16_t)val_mv;
	// frame->payload_len = 2;
	// memcpy(buff, frame, sizeof(frame));
	// uint16_t tx_value = (int16_t)val_mv;
	LOG_INF("Transmitted actual value: %#x, %hd", buff->value, buff->value);
	// buff[sizeof(frame)] = tx_value; /// temperature value
	ret = usb_write(RO_EP_IN_ADDR, buff, 5, &read_bytes);
	if (ret < 0)
	{
		LOG_ERR("usb_write() failed");
		return;
	}
	LOG_INF("Wrote %d bytes\n", read_bytes);
	k_free(buff);
}

void led_setup(const struct gpio_dt_spec *spec)
{
	int ret;
	if (!device_is_ready(spec->port))
	{
		printk("Error: %s device is not ready\n", spec->port->name);
		return;
	}

	ret = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
	if (ret != 0)
	{
		printk("Error %d: failed to configure pin %d \n",
			   ret, spec->pin);
		return;
	}
}