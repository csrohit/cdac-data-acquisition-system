#include <linux/module.h>
#include <linux/usb.h>
#include <linux/slab.h>

#ifndef __USB_CUSTOM_H__
#define __USB_CUSTOM_H__

#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x01
#define BULK_EP_IN 0x81

extern struct usb_driver ro_driver;
extern struct proc_ops proc_fops;
extern struct proc_dir_entry *ent;
extern struct usb_class_driver ro_class;

typedef struct struct_ro_usb_dev
{
    struct usb_device *udev;
    struct kref kref;
    uint8_t is_connected;
    uint8_t *bulk_in_buffer;
    size_t bulk_in_size;
    struct usb_interface *interface;
    struct list_head list;
} ro_usb_dev_t;

extern ro_usb_dev_t list_head;


void ro_dev_release(struct kref *kref);
#endif