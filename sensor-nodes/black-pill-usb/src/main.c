/*
 * Copyright (c) 2022 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <zephyr/device.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usb_ch9.h>

#define RO_EP_OUT_ADDR 0X01
#define RO_EP_IN_ADDR 0X81

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
		.bInterfaceClass = USB_BCC_VENDOR,
		.bInterfaceSubClass = 0,
		.bInterfaceProtocol = 0,
		.iInterface = 0},

	// data endpoint out
	.if0_out_ep = {.bLength = sizeof(struct usb_ep_descriptor), .bDescriptorType = USB_DESC_ENDPOINT, .bEndpointAddress =RO_EP_OUT_ADDR, .bInterval = 0x00, .wMaxPacketSize = 0x0040, .bmAttributes = USB_DC_EP_BULK},
	.if0_in_ep = {.bLength = sizeof(struct usb_ep_descriptor), .bDescriptorType = USB_DESC_ENDPOINT, .bEndpointAddress = RO_EP_IN_ADDR, .bInterval = 0x00, .wMaxPacketSize = 0x0040, .bmAttributes = USB_DC_EP_BULK}

};

void ro_out_cb(uint8_t ep,
			   enum usb_dc_ep_cb_status_code cb_status);

void ro_in_cb(uint8_t ep,
			   enum usb_dc_ep_cb_status_code cb_status);
static struct usb_ep_cfg_data ep_cfg[] = {
	{
		.ep_cb = ro_out_cb,
	 	.ep_addr = RO_EP_OUT_ADDR
	 },
	 {
		.ep_cb = ro_in_cb,
		.ep_addr = RO_EP_IN_ADDR
	 }
};

main(void)
{
	printk("Hello World! %s\n", "Rohit");
}

void ro_out_cb(uint8_t ep,
			   enum usb_dc_ep_cb_status_code cb_status)
{
}
void ro_in_cb(uint8_t ep,
			   enum usb_dc_ep_cb_status_code cb_status)
{
}
