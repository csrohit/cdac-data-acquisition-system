#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x01
#define BULK_EP_IN 0x81
uint8_t maxPacketSize = 0x40;

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

/**
 * @brief Routine to be called when read() system call is invoked
 *
 * @param pfile pointer to file
 * @param pbuf pointer to userspace buffer
 * @param len no. of bytes to be read
 * @param poffset offset from beginning
 * @return ssize_t  -1 if failed else no. of bytes successfully read
 */
ssize_t ro_read(struct file *pfile, char __user *pbuf, size_t len, loff_t *poffset);


/**
 * @brief Callback for write operation by user space program
 * Write data to bulk endpoint
 * @param p_file pointer to struct file
 * @param p_buff pointer to userspace buffer to be written
 * @param len number of bytes to be written
 * @param _offset offset of file
 * @return ssize_t number of bytes successfully written
 */
ssize_t ro_write(struct file *pfile, const char __user *pbuf, size_t len, loff_t *poffset);

/**
 * @brief Callback function when read system call is called by userspace program
 * - Allocate memory required to hold a device and initialize data structures
 * @param p_inode pointer to inode
 * @param p_file pointer to struct file
 * @return int 0 if success else negative error status
 */
int ro_open(struct inode *pinode, struct file *pfile);

/**
 * @brief Routine to be called when close() system call is invoked
 * 
 * @param pfile pointer to file
 * @param id owener id
 * @return int 0 if success else negative error code
 */
int ro_flush (struct file * pfile, fl_owner_t id);


/**
 * @brief callback function when close() system call is called by userspace program
 *  - Release resource allocated in open() system call
 * @param p_inode pointer to inode
 * @param p_file opinter to struct file
 * @return int 0 if success else negative error status
 */
int ro_release(struct inode *pinode, struct file *pfile);



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

static struct usb_device *ro_device;
static struct usb_class_driver ro_class;
static struct file_operations ro_fops = {
    .owner = THIS_MODULE,
    .open = ro_open,
    .read = ro_read,
    .write = ro_write,
    .flush = ro_flush,
    .release = ro_release};

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
        printk(KERN_ERR "%s usb_register() failed for driver (%s). Error number %d\n", THIS_MODULE->name, ro_driver.name, ret);
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
    printk(KERN_INFO "%s: exit module\n", THIS_MODULE->name);
    /* deregister this driver with the USB subsystem */
    usb_deregister(&ro_driver);
    printk(KERN_INFO "%s: Deregistered driver (%s) with USB subsystem\n", THIS_MODULE->name, ro_driver.name);
}

int ro_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    int ret;
    uint8_t i;
    struct usb_host_endpoint *endpoint;
    struct usb_host_interface *active_config;
    printk(KERN_INFO "%s: probe called\n", THIS_MODULE->name);

    // current active interface
    active_config = intf->cur_altsetting;
    printk(KERN_INFO "%s: plugged device-> %04X:%04X \n", THIS_MODULE->name, id->idVendor, id->idProduct);
    printk(KERN_INFO "%s: interface no.-> %04X \n", THIS_MODULE->name, active_config->desc.bInterfaceNumber);
    printk(KERN_INFO "%s: total endpoints-> %04X\n", THIS_MODULE->name, active_config->desc.bNumEndpoints);
    printk(KERN_INFO "%s: interface class-> %04X\n", THIS_MODULE->name, active_config->desc.bInterfaceClass);
    // printk(KERN_INFO "%s: exit module\n", THIS_MODULE->name);
    for (i = 0; i < active_config->desc.bNumEndpoints; i++)
    {
        endpoint = (active_config->endpoint + i);
        printk(KERN_INFO "%s: endpoint address-> %#04x\n", THIS_MODULE->name, endpoint->desc.bEndpointAddress);
    }

    ro_device = interface_to_usbdev(intf);
    ro_class.name = "usb/node%d";
    ro_class.fops = &ro_fops;
    ret = usb_register_dev(intf, &ro_class);
    if (ret < 0)
    {
        printk(KERN_ERR "%s usb_register_dev() failed to assign minor\n", THIS_MODULE->name);
    }
    else
    {
        printk(KERN_ERR "%s assigned minor number-> %d\n", THIS_MODULE->name, intf->minor);
    }
    return 0;
}

void ro_disconnect(struct usb_interface *intf)
{
    printk(KERN_INFO "%s: disconnect called\n", THIS_MODULE->name);
    usb_deregister_dev(intf, &ro_class);
    pr_info("Deregistered driver (%s) from usb subsystem\n", ro_class.name);
    printk(KERN_INFO "%s: removed interface-> %#04X\n", THIS_MODULE->name, intf->cur_altsetting->desc.bInterfaceNumber);
}


ssize_t ro_read(struct file *p_file, char __user *p_buff, size_t max_len, loff_t *_offset)
{
    printk(KERN_INFO "%s: read called\n", THIS_MODULE->name);
    return -1;
}

ssize_t ro_write(struct file *p_file, const char __user *p_buff, size_t len, loff_t *_offset)
{
    int pipe, bytes_to_write, nbytes, ret;
    void *k_buff;

    printk(KERN_INFO "%s: write called (%lu bytes)\n", THIS_MODULE->name, len);

    bytes_to_write = MIN(maxPacketSize, len);

    // allocate kernel space buffer for holding data to be written
    k_buff = kmalloc(bytes_to_write, GFP_KERNEL);
    if (IS_ERR(k_buff))
    {
        // could not allocate buffer in kernel space
        printk(KERN_ERR "%s kmalloc() failed\n", THIS_MODULE->name);
        return -ENOMEM;
    }

    printk(KERN_INFO "%s: allocated %u bytes for kernel data buffer\n", THIS_MODULE->name, bytes_to_write);

    // copy_from_user returens no. of bytes no copied
    nbytes = bytes_to_write - copy_from_user(k_buff, p_buff, bytes_to_write);
    if (nbytes == 0)
    {
        pr_err("%s no data to write\n", THIS_MODULE->name);
        kfree(k_buff);
        return -EFAULT;
    }

    // get bulk out pipe
    pipe = usb_sndbulkpipe(ro_device, BULK_EP_OUT);

    // write data to usb device
    ret = usb_bulk_msg(ro_device, pipe, k_buff, bytes_to_write, &nbytes, 5000);
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


int ro_open(struct inode *p_inode, struct file *p_file)
{
    printk(KERN_INFO "%s: open called\n", THIS_MODULE->name);
    return 0;
}

int ro_flush (struct file * pfile, fl_owner_t id){
    printk(KERN_INFO "%s: flush called\n", THIS_MODULE->name);
    return 0;
}

int ro_release(struct inode *p_inode, struct file *p_file)
{
    printk(KERN_INFO "%s: release called\n", THIS_MODULE->name);
    return 0;
}

module_init(ro_init);
module_exit(ro_exit);
MODULE_AUTHOR("Rohit Nimkar <nehalnimkar@gmail.com>");
MODULE_DESCRIPTION("USB Device Driver for our project");

MODULE_LICENSE("GPL");