#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/uaccess.h>

/**
 * @brief Function to be called when usb device is attached to the system matching the id table elntry
 * 
 * @param intf pointer to matching usb interface
 * @param id usb device id
 * @return int 0 if success else negative error code
 */
int ro_probe(struct usb_interface *intf, const struct usb_device_id *id);

/**
 * @brief Function to be called when registered device is removed form the system
 * 
 * @param intf pointer to registered interface
 */
void ro_disconnect(struct usb_interface *intf);

static struct usb_device_id ro_id_table[] =
    {
        //{ USB_DEVICE(0x0951, 0x1607) },
        {USB_DEVICE(0x2fff, 0x0001)},
        {} /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, ro_id_table); // register id table with usb core

static struct usb_driver ro_driver = {
    .name = "custom-driver",
    .probe = ro_probe,
    .id_table = ro_id_table,
    .disconnect = ro_disconnect};

/**
 * @brief Initialization function for module
 *
 * @return -1 if failed else 0
 */
static __init int ro_init(void)
{
    int ret;

    printk(KERN_INFO "%s: init module\n", THIS_MODULE->name);

    /* register this driver with the USB subsystem */
    ret = usb_register(&ro_driver);
    if (ret < 0)
    {
        printk(KERN_ERR "%s usb_register() failed for driver (%s). Error number %d\n",THIS_MODULE->name, ro_driver.name, ret);
        return -1;
    }
    printk(KERN_INFO " %s: Registered driver (%s) with USB subsystem\n", THIS_MODULE->name, ro_driver.name);
    return 0;
}

/**
 * @brief Deinitialization function for module
 *
 * @return
 */
static __exit void ro_exit(void)
{
    printk(KERN_INFO"%s: exit module\n", THIS_MODULE->name);
    /* deregister this driver with the USB subsystem */
    usb_deregister(&ro_driver);
    printk(KERN_INFO "%s: Deregistered driver (%s) with USB subsystem\n", THIS_MODULE->name, ro_driver.name);
}

int ro_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    // int i, ret;

    struct usb_host_interface *active_config;
    printk(KERN_INFO"%s: probe called\n", THIS_MODULE->name);

    // current active interface 
    active_config = intf->cur_altsetting;
    printk(KERN_INFO"%s: plugged device-> %04X:%04X \n", THIS_MODULE->name, id->idVendor,id->idProduct);
    printk(KERN_INFO"%s: interface no.-> %04X \n", THIS_MODULE->name,active_config->desc.bInterfaceNumber);
    printk(KERN_INFO"%s: total endpoints-> %04X\n", THIS_MODULE->name, active_config->desc.bNumEndpoints);
    printk(KERN_INFO"%s: interface class-> %04X\n", THIS_MODULE->name, active_config->desc.bInterfaceClass);
    printk(KERN_INFO"%s: exit module\n", THIS_MODULE->name);  
    return 0;
}

void ro_disconnect(struct usb_interface *intf){
    printk(KERN_INFO"%s: disconnect called\n", THIS_MODULE->name);
    printk(KERN_INFO"%s: removed interface-> %#04X\n", THIS_MODULE->name,intf->cur_altsetting->desc.bInterfaceNumber);
}

module_init(ro_init);
module_exit(ro_exit);
MODULE_AUTHOR("Rohit Nimkar <nehalnimkar@gmail.com>");
MODULE_DESCRIPTION("USB Device Driver for our project");

MODULE_LICENSE("GPL");