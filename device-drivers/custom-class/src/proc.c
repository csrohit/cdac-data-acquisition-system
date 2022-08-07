#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include "usb_custom.h"

ro_usb_dev_t list_head;

/**
 * @brief Routine to be called when read() system call is invoked for proc fs node
 *
 * @param pfile pointer to file
 * @param pbuf pointer to userspace buffer
 * @param len no. of bytes to be read
 * @param poffset offset from beginning
 * @return ssize_t  -1 if failed else no. of bytes successfully read
 */
static ssize_t proc_read(struct file *pfile, char __user *pbuf, size_t len, loff_t *poffset);

/**
 * @brief Callback for write operation by user space program for proc fs node
 * Write data to bulk endpoint
 * @param p_file pointer to struct file
 * @param p_buff pointer to userspace buffer to be written
 * @param len number of bytes to be written
 * @param _offset offset of file
 * @return ssize_t number of bytes successfully written
 */
static ssize_t proc_write(struct file *pfile, const char __user *pbuf, size_t len, loff_t *poffset);

static int proc_open(struct inode *p_inode, struct file *p_file);
static int proc_release(struct inode *p_inode, struct file *p_file);

struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_release = proc_release,
    .proc_read = proc_read,
    .proc_write = proc_write};

struct proc_dir_entry *ent = NULL;

static ssize_t proc_read(struct file *p_file, char __user *p_buff, size_t max_len, loff_t *_offset)
{
    unsigned long ret;
    ro_usb_dev_t *trav;
    pr_info("%s: Proc read called (%ld)\n", THIS_MODULE->name, max_len);
    if (*_offset == 0)
    {
        list_for_each_entry(trav, &list_head.list, list)
        {
            printk("%s:  %s\n", THIS_MODULE->name, trav->udev->devpath);
        }

        char *data = "THis is my data";
        ssize_t len = strlen(data);
        *_offset = len;
        ret = copy_to_user(p_buff, data, MIN(len, max_len));
        pr_info("%s: copied data to user space\n", THIS_MODULE->name);
        return MIN(len, max_len);
    }
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
