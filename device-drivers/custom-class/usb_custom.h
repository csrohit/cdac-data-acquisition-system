#include <linux/module.h>
#include <linux/usb.h>
#ifndef __USB_CUSTOM_H__
#define __USB_CUSTOM_H__

#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x01
#define BULK_EP_IN 0x81

typedef struct struct_ro_usb_dev
{
    struct usb_device *udev;
    struct kref kref;
    uint8_t is_connected;
    uint8_t *bulk_in_buffer;
    size_t bulk_in_size;
    struct usb_interface *interface;
} ro_usb_dev_t;

/**
 * @brief Initialization function for module
 *
 * @return -1 if failed else 0
 */
static __init int ro_init(void);

/**
 * @brief Deinitialization function for module
 *
 * @return
 */
static __exit void ro_exit(void);

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

// /**
//  * @brief Routine to be called when close() system call is invoked
//  *
//  * @param pfile pointer to file
//  * @param id owener id
//  * @return int 0 if success else negative error code
//  */
// int ro_file_flush (struct file * pfile, fl_owner_t id);

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
static void ro_dev_release(struct kref *kref);

#endif