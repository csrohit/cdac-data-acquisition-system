#include <linux/kernel.h>
#include <linux/uaccess.h>
#include "usb_custom.h"

/**
 * @brief Routine to be called when read() system call is invoked
 *
 * @param pfile pointer to file
 * @param pbuf pointer to userspace buffer
 * @param len no. of bytes to be read
 * @param poffset offset from beginning
 * @return ssize_t  -1 if failed else no. of bytes successfully read
 */
static ssize_t ro_file_read(struct file *pfile, char __user *pbuf, size_t len, loff_t *poffset);

/**
 * @brief Callback for write operation by user space program
 * Write data to bulk endpoint
 * @param p_file pointer to struct file
 * @param p_buff pointer to userspace buffer to be written
 * @param len number of bytes to be written
 * @param _offset offset of file
 * @return ssize_t number of bytes successfully written
 */
static ssize_t ro_file_write(struct file *pfile, const char __user *pbuf, size_t len, loff_t *poffset);

/**
 * @brief Callback function when read system call is called by userspace program
 * - Allocate memory required to hold a device and initialize data structures
 * @param p_inode pointer to inode
 * @param p_file pointer to struct file
 * @return int 0 if success else negative error status
 */
static int ro_file_open(struct inode *pinode, struct file *pfile);

/**
 * @brief callback function when close() system call is called by userspace program
 *  - Release resource allocated in open() system call
 * @param p_inode pointer to inode
 * @param p_file opinter to struct file
 * @return int 0 if success else negative error status
 */
static int ro_file_release(struct inode *pinode, struct file *pfile);

/**
 * @brief Release resources allocated for device
 *
 * @param kref pointer to kref
 */
void ro_dev_release(struct kref *kref);

static struct file_operations ro_fops = {
    .owner = THIS_MODULE,
    .open = ro_file_open,
    .read = ro_file_read,
    .write = ro_file_write,
    .release = ro_file_release};
struct usb_class_driver ro_class = {
    .name = "usb/node%d",
    .fops = &ro_fops,
    .minor_base = 0};

uint8_t maxPacketSize = 0x40;

static ssize_t ro_file_read(struct file *p_file, char __user *p_buff, size_t max_len, loff_t *_offset)
{
    static ro_usb_dev_t *dev;
    static int pipe, ret, nbytes;
    static size_t bytes_to_read;
    static char *k_buff;
    pr_info("%s: read called\n", THIS_MODULE->name);
    dev = p_file->private_data;
    if (IS_ERR(dev))
    {
        pr_err("%s: device is NULL in file pointer\n", THIS_MODULE->name);
        return -ENODEV;
    }

    if (!dev->is_connected)
    {
        pr_err("%s: device is disconnected\n", THIS_MODULE->name);
        return -ENODEV;
    }

    pipe = usb_rcvbulkpipe(dev->udev, BULK_EP_IN);
    bytes_to_read = MIN(max_len, maxPacketSize);

    k_buff = (uint8_t *)kmalloc(bytes_to_read, GFP_KERNEL);
    pr_info("%s: Reading %ld bytes from usb\n", THIS_MODULE->name, bytes_to_read);

    // write data to usb device
    ret = usb_bulk_msg(dev->udev, pipe, k_buff, bytes_to_read, &nbytes, 10000);
    if (ret < 0)
    {
        pr_err("%s: Bulk message returned %d\n", THIS_MODULE->name, ret);
        kfree(k_buff);
        return -EFAULT;
    }
    pr_info("%s: Read %d bytes from usb\n", THIS_MODULE->name, nbytes);

    if (copy_to_user(p_buff, k_buff, nbytes))
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
