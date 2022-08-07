#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>

#include "usb_custom.h"

uint8_t maxPacketSize = 0x40;

static struct proc_dir_entry *ent = NULL;
static struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_release = proc_release,
    .proc_read = proc_read,
    .proc_write = proc_write};

static struct usb_device_id ro_id_table[] =
    {
        {USB_DEVICE(0x2fff, 0x0001)},
        {} /* Terminating entry */
};
MODULE_DEVICE_TABLE(usb, ro_id_table); // register id table with usb core

static struct usb_driver ro_driver = {
    .name = "custom-driver",
    .probe = ro_probe,
    .id_table = ro_id_table,
    .disconnect = ro_disconnect};

static struct file_operations ro_fops = {
    .owner = THIS_MODULE,
    .open = ro_file_open,
    .read = ro_file_read,
    .write = ro_file_write,
    .release = ro_file_release};
static struct usb_class_driver ro_class = {
    .name = "usb/node%d",
    .fops = &ro_fops,
    .minor_base = 0};

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

static ssize_t proc_read(struct file *p_file, char __user *p_buff, size_t max_len, loff_t *_offset)
{
    pr_info("%s: Proc read called (%ld@%ld)\n", THIS_MODULE->name, max_len, *_offset);
    if (*_offset == 0)
    {
        char *data = "THis is my data";
        ssize_t len = strlen(data);
        *_offset = len;
        copy_to_user(p_buff, data, MIN(len, max_len));
        pr_info("%s: copied data to user space\n", THIS_MODULE->name);
        return MIN(len, max_len);
    }
    pr_info("%s: No data available\n", THIS_MODULE->name);
    return 0;
}

static int proc_open(struct inode *p_inode, struct file *p_file)
{
    pr_info("%s: Proc open called\n", THIS_MODULE->name);
    return 0;
}
static int proc_release(struct inode *p_inode, struct file *p_file)
{
    pr_info("%s: Proc release called\n", THIS_MODULE->name);
    return 0;
}
static ssize_t proc_write(struct file *p_file, const char __user *p_buff, size_t len, loff_t *_offset)
{
    pr_info("%s: Proc write called\n", THIS_MODULE->name);
    return len;
}

static ssize_t ro_file_read(struct file *p_file, char __user *p_buff, size_t max_len, loff_t *_offset)
{
    static ro_usb_dev_t *dev;
    static int pipe, ret, nbytes;
    static size_t bytes_to_read;
    static void *k_buff;
    printk(KERN_INFO "%s: read called\n", THIS_MODULE->name);
    dev = p_file->private_data;
    if (IS_ERR(dev))
    {
        printk(KERN_ERR "%s: device is NULL in file pointer\n", THIS_MODULE->name);
        return -ENODEV;
    }

    if (!dev->is_connected)
    {
        printk(KERN_ERR "%s: device is disconnected\n", THIS_MODULE->name);
        return -ENODEV;
    }

    pipe = usb_rcvbulkpipe(dev->udev, BULK_EP_IN);
    bytes_to_read = MIN(max_len, maxPacketSize);

    k_buff = kmalloc(bytes_to_read, GFP_KERNEL);
    pr_info("%s: Reading %ld bytes from usb\n", THIS_MODULE->name, bytes_to_read);
    pr_info("%s: Allocated %ld bytes for buffer\n", THIS_MODULE->name, bytes_to_read);

    // write data to usb device
    ret = usb_bulk_msg(dev->udev, pipe, k_buff, bytes_to_read, &nbytes, 10000);
    if (ret < 0)
    {
        printk(KERN_ERR "%s: Bulk message returned %d\n", THIS_MODULE->name, ret);
        kfree(k_buff);
        return -EFAULT;
    }

    pr_info("%s: Read %d bytes from usb\n", THIS_MODULE->name, nbytes);

    if (copy_to_user(p_buff, k_buff, 4))
    {
        pr_err("%s: Error while copying data to user space\n", THIS_MODULE->name);
        kfree(k_buff);
        return -EFAULT;
    }
    ret = nbytes;
    pr_info("%s: freeing memory of k_buff\n", THIS_MODULE->name);
    kfree(k_buff);
    return ret;
}

static ssize_t ro_file_write(struct file *p_file, const char __user *p_buff, size_t len, loff_t *_offset)
{
    static int pipe, bytes_to_write, nbytes, ret;
    static void *k_buff;
    static ro_usb_dev_t *dev;

    printk(KERN_INFO "%s: write called (%lu bytes)\n", THIS_MODULE->name, len);

    dev = p_file->private_data;
    if (IS_ERR(dev))
    {
        printk(KERN_ERR "%s: device is NULL in file pointer\n", THIS_MODULE->name);
        return -ENODEV;
    }

    if (!dev->is_connected)
    {
        printk(KERN_ERR "%s: device is disconnected\n", THIS_MODULE->name);
        return -ENODEV;
    }

    bytes_to_write = MIN(maxPacketSize, len);

    // allocate kernel space buffer for holding data to be written
    k_buff = kmalloc(bytes_to_write, GFP_KERNEL);
    if (IS_ERR(k_buff))
    {
        // could not allocate buffer in kernel space
        printk(KERN_ERR "%s kmalloc() failed\n", THIS_MODULE->name);
        return -ENOMEM;
    }
    // copy_from_user returens no. of bytes no copied
    nbytes = bytes_to_write - copy_from_user(k_buff, p_buff, bytes_to_write);
    if (nbytes == 0)
    {
        pr_err("%s no data to write\n", THIS_MODULE->name);
        kfree(k_buff);
        return -EFAULT;
    }

    // get bulk out pipe
    pipe = usb_sndbulkpipe(dev->udev, BULK_EP_OUT);

    // write data to usb device
    ret = usb_bulk_msg(dev->udev, pipe, k_buff, bytes_to_write, &nbytes, 5000);
    if (ret)
    {
        printk(KERN_ERR "%s: Bulk message returned %d\n", THIS_MODULE->name, ret);
        kfree(k_buff);
        return -EFAULT;
    }
    printk(KERN_INFO "%s: Wrote bytes: %d\n", THIS_MODULE->name, nbytes);

    // free allocated kernel buffer
    kfree(k_buff);
    return nbytes;
}

static int ro_file_open(struct inode *p_inode, struct file *p_file)
{
    int subminor;
    static ro_usb_dev_t *dev;
    static struct usb_interface *intf;

    printk(KERN_INFO "%s: open called\n", THIS_MODULE->name);

    // get minor number
    subminor = iminor(p_inode);

    // find interface
    intf = usb_find_interface(&ro_driver, subminor);

    // find usb device
    dev = usb_get_intfdata(intf);

    // increment device reference count
    kref_get(&dev->kref);

    // store reference to dev in file pointer
    p_file->private_data = dev;
    return 0;
}

static int ro_file_release(struct inode *p_inode, struct file *p_file)
{
    static int ret;
    static ro_usb_dev_t *dev;

    printk(KERN_INFO "%s: release called\n", THIS_MODULE->name);

    // get usb device
    dev = p_file->private_data;

    if (IS_ERR(dev))
    {
        printk(KERN_ERR "%s: couldn't find device in file pointer\n", THIS_MODULE->name);
        return -ENODEV;
    }

    p_file->private_data = NULL;

    // decrement reference count
    ret = kref_put(&dev->kref, ro_dev_release);
    printk(KERN_INFO "%s: [release] Kref put => %d\n", THIS_MODULE->name, ret);
    return 0;
}

static void ro_dev_release(struct kref *kref)
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