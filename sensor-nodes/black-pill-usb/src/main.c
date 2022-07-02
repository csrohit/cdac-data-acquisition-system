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

LOG_MODULE_REGISTER(usb_driver, CONFIG_LOG_DEFAULT_LEVEL);

// #define RO_EP_OUT_ADDR 0X01
// #define RO_EP_IN_ADDR 0X81

// void ro_out_cb(uint8_t ep,
// 			   enum usb_dc_ep_cb_status_code cb_status);

// void ro_in_cb(uint8_t ep,
			//    enum usb_dc_ep_cb_status_code cb_status);
// struct ro_usb_config
// {
// 	struct usb_if_descriptor if0;
// 	struct usb_ep_descriptor if0_out_ep;
// 	struct usb_ep_descriptor if0_in_ep;
// } __packed;

// USBD_CLASS_DESCR_DEFINE(primary, 0)
// struct ro_usb_config ro_cfg = {
// 	// interface descriptor
// 	.if0 = {
// 		.bLength = sizeof(struct usb_if_descriptor),
// 		.bDescriptorType = USB_DESC_INTERFACE,
// 		.bInterfaceNumber = 0,
// 		.bAlternateSetting = 0,
// 		.bNumEndpoints = 2,
// 		.bInterfaceClass = USB_BCC_VENDOR,
// 		.bInterfaceSubClass = 0,
// 		.bInterfaceProtocol = 0,
// 		.iInterface = 0},

// 	// data endpoint out
// 	.if0_out_ep = {.bLength = sizeof(struct usb_ep_descriptor), .bDescriptorType = USB_DESC_ENDPOINT, .bEndpointAddress =RO_EP_OUT_ADDR, .bInterval = 0x00, .wMaxPacketSize = 0x0040, .bmAttributes = USB_DC_EP_BULK},
// 	.if0_in_ep = {.bLength = sizeof(struct usb_ep_descriptor), .bDescriptorType = USB_DESC_ENDPOINT, .bEndpointAddress = RO_EP_IN_ADDR, .bInterval = 0x00, .wMaxPacketSize = 0x0040, .bmAttributes = USB_DC_EP_BULK}

// };

// static struct usb_ep_cfg_data ep_cfg[] = {
// 	{
// 		.ep_cb = ro_out_cb,
// 	 	.ep_addr = RO_EP_OUT_ADDR
// 	 },
// 	 {
// 		.ep_cb = ro_in_cb,
// 		.ep_addr = RO_EP_IN_ADDR
// 	 }
// };

void main(void)
{
	const struct device *dev;
	uint32_t dtr = 0U;
	int ret;

	dev = DEVICE_DT_GET_ONE(zephyr_cdc_acm_uart);
	if(!device_is_ready(dev)){
		LOG_ERR("CDC ACM device not ready");
		return;
	}
	LOG_INF("CDC ACM device is ready");

	ret = usb_enable(NULL);
	if(ret < 0){
		LOG_ERR("usb_enable() failed");
		return;
	}
	LOG_INF("usb device enabled");

	printk("Hello World! %s\n", "Rohit");


	LOG_INF("Wait for DTR");

	while (true) {
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
		if (dtr) {
			LOG_INF("Received DTR");
			break;
		} else {
			/* Give CPU resources to low priority threads. */
			k_sleep(K_MSEC(100));
		}
	}

}

// void ro_out_cb(uint8_t ep,
// 			   enum usb_dc_ep_cb_status_code cb_status)
// {
// }
// void ro_in_cb(uint8_t ep,
// 			   enum usb_dc_ep_cb_status_code cb_status)
// {
// }
