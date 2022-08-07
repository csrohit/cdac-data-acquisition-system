#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include "usb_custom.h"


/**
 * @brief Function to be called when usb device is attached to the system matching the id table elntry
 *
 * @param intf pointer to matching usb interface
 * @param id usb device id
 * @return int 0 if success else negative error code
 */
static int ro_probe(struct usb_interface *intf, const struct usb_device_id *id);

/**
 * @brief Function to be called when registered device is removed form the system
 *
 * @param intf pointer to registered interface
 */
static void ro_disconnect(struct usb_interface *intf);


struct usb_device_id ro_id_table[] =
    {
        {USB_DEVICE(0x2fff, 0x0001)},
        {} /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, ro_id_table); // register id table with usb core

struct usb_driver ro_driver = {
    .name = "custom-driver",
    .probe = ro_probe,
    .id_table = ro_id_table,
    .disconnect = ro_disconnect};

static __init int ro_init(void)
{
    int ret;

    printk(KERN_INFO "%s: init module\n", THIS_MODULE->name);

    ent = proc_create("rohit_usb", 0666, NULL, &proc_fops);

    if (IS_ERR(ent))
    {
        pr_err("%s: proc_create() failed\n", THIS_MODULE->name);
        return -1;
    }
    /* register this driver with the USB subsystem */
    ret = usb_register(&ro_driver);
    if (ret < 0)
    {
        printk(KERN_ERR "%s usb_register() failed for driver (%s). Error number %d\n", THIS_MODULE->name, ro_driver.name, ret);
        return -1;
    }
    printk(KERN_INFO " %s: Registered driver (%s) with USB subsystem\n", THIS_MODULE->name, ro_driver.name);
    return 0;
}

static __exit void ro_exit(void)
{
    printk(KERN_INFO "%s: exit module\n", THIS_MODULE->name);
    /* deregister this driver with the USB subsystem */
    usb_deregister(&ro_driver);

    proc_remove(ent);
    printk(KERN_INFO "%s: Deregistered driver (%s) with USB subsystem\n", THIS_MODULE->name, ro_driver.name);
}



static int ro_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    int ret;
    uint8_t i;
    struct usb_host_endpoint *endpoint;
    struct usb_host_interface *active_config;
    ro_usb_dev_t *ro_dev;
    printk(KERN_INFO "%s: probe called\n", THIS_MODULE->name);

    // current active interface
    active_config = intf->cur_altsetting;
    printk(KERN_INFO "%s: plugged device-> %04X:%04X \n", THIS_MODULE->name, id->idVendor, id->idProduct);
    printk(KERN_INFO "%s: interface no.-> %04X \n", THIS_MODULE->name, active_config->desc.bInterfaceNumber);
    printk(KERN_INFO "%s: total endpoints-> %04X\n", THIS_MODULE->name, active_config->desc.bNumEndpoints);
    printk(KERN_INFO "%s: interface class-> %04X\n", THIS_MODULE->name, active_config->desc.bInterfaceClass);
    for (i = 0; i < active_config->desc.bNumEndpoints; i++)
    {
        endpoint = (active_config->endpoint + i);
        printk(KERN_INFO "%s: endpoint address-> %#04x\n", THIS_MODULE->name, endpoint->desc.bEndpointAddress);
    }

    ro_dev = kzalloc(sizeof(ro_usb_dev_t), GFP_KERNEL);
    if (IS_ERR(ro_dev))
    {
        printk(KERN_ERR "%s: failed to allocate memory to usb device\n", THIS_MODULE->name);
        return -ENOMEM;
    }

    // initialize kref object
    kref_init(&ro_dev->kref);

    // increment ref count of interface
    ro_dev->interface = intf;
    usb_get_intf(intf);

    // increment ref count for usb device
    ro_dev->udev = interface_to_usbdev(intf);
    usb_get_dev(ro_dev->udev);

    // set pointer to data in iterface (device)
    usb_set_intfdata(intf, ro_dev);

    ret = usb_register_dev(intf, &ro_class);
    if (ret < 0)
    {
        usb_set_intfdata(intf, NULL);
        printk(KERN_ERR "%s usb_register_dev() failed to assign minor\n", THIS_MODULE->name);
        goto dev_reg_err;
    }

    ro_dev->is_connected = 1;
    dev_info(
        &intf->dev,
        "%s: USB device now attached to node%d",
        THIS_MODULE->name,
        intf->minor);
    return 0;

dev_reg_err:
    kref_put(&ro_dev->kref, ro_dev_release);
    return ret;
}

static void ro_disconnect(struct usb_interface *intf)
{
    int ret;
    static ro_usb_dev_t *ro_dev;

    printk(KERN_INFO "%s: disconnect called\n", THIS_MODULE->name);

    ro_dev = usb_get_intfdata(intf);
    usb_set_intfdata(intf, NULL);

    ro_dev->is_connected = 0;

    usb_deregister_dev(intf, &ro_class);
    pr_info("%s: Deregistered device (%s) from usb subsystem\n", THIS_MODULE->name, ro_class.name);
    printk(KERN_INFO "%s: removed interface-> %#04X\n", THIS_MODULE->name, intf->cur_altsetting->desc.bInterfaceNumber);

    ret = kref_put(&ro_dev->kref, ro_dev_release);
    printk(KERN_INFO "%s: [disconnect] Kref put => %d\n", THIS_MODULE->name, ret);
}


void ro_dev_release(struct kref *kref)
{
    ro_usb_dev_t *ro_dev;
    ro_dev = container_of(kref, ro_usb_dev_t, kref);
    printk(KERN_INFO "%s: ro_dev_release called\n", THIS_MODULE->name);

    // decrement usage count of usb device
    usb_put_dev(ro_dev->udev);

    // decrement usage count of interface
    usb_put_intf(ro_dev->interface);

    // free memory allocated to device
    pr_info("%s: releasing resources for device\n", THIS_MODULE->name);
    kfree(ro_dev);
}


module_init(ro_init);
module_exit(ro_exit);
MODULE_AUTHOR("Rohit Nimkar <nehalnimkar@gmail.com>");
MODULE_DESCRIPTION("USB Device Driver for our project");

MODULE_LICENSE("GPL");