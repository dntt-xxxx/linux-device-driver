/***************************************************************************//**
*  \file       driver.c
*
*  \details    Simple linux driver (Statically allocating the Major and Minor number)
*
*  \author     EmbeTronicX
*
* *******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>
#include <linux/err.h>

int value = 0;

#define WR_VALUE _IOW('testioctl','w',int32_t*)
#define RD_VALUE _IOR('testioctl','r',int32_t*)

//creating the dev with our custom major and minor number
dev_t dev = MKDEV(233, 2);
static struct class *dev_class;

// void cdev_init(struct cdev *cdev, const struct file_operations *fops)
static struct cdev s_cdev;
static ssize_t test_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t test_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static int test_open (struct inode *, struct file *);
static int test_release (struct inode *, struct file *);
long test_ioctl (struct file *, unsigned int, unsigned long);

struct file_operations fops = { 
        .owner          = THIS_MODULE,
        .read           = test_read,
        .write          = test_write,
        .open           = test_open,
        .unlocked_ioctl = test_ioctl,
        .release        = test_release,
};
/*
** Module Init function
*/
static int __init hello_world_init(void)
{
    register_chrdev_region(dev, 1, "test-ioctl-dev");
    printk(KERN_INFO "STATIC: Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

    cdev_init(&s_cdev, &fops);
    if (cdev_add((struct cdev *)&s_cdev, &dev,1)<0)   
    {
        printk(KERN_ERR "cdev_add fail\n");
    }
    /*Creating struct class*/
    if(IS_ERR(dev_class = class_create(THIS_MODULE,"test_ioctl_class"))){
        pr_err("Cannot create the struct class\n");
        goto des_class;
    }    
    /*Creating device*/
    if(IS_ERR(device_create(dev_class,NULL,&dev,NULL,"test_ioctl_dev"))){
        pr_err("Cannot create the Device 1\n");
        goto des_device;
    }
    pr_info("Device Driver Insert...Done!!!\n");
    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;

des_device:
    class_destroy(dev_class);
des_class:
    unregister_chrdev_region(&dev,1);
    return -1;

}

/*
** Module exit function
*/
static void __exit hello_world_exit(void)
{
    device_destroy(dev_class,&dev);
    class_destroy(dev_class);
    cdev_del(&s_cdev);
    unregister_chrdev_region(&dev, 1);
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");

}
static ssize_t test_read(struct file *pfile, char __user *buf, size_t len, loff_t *off)
{
    pr_info("test_read: \n");
    return 0;
}

// static ssize_t test_write(struct file *filp, char __user *buf, size_t, loff_t *off)
static ssize_t test_write(struct file *filp, const char *buf, size_t len, loff_t * off)
{
    pr_info("test_write: \n");
    return 0;

}
static int test_open (struct inode *pinode, struct file *pfile)
{
    pr_info("test_open: \n");
    return 0;
}
static int test_release (struct inode *pinode, struct file *pfile)
{
    pr_info("test_release: \n");
    return 0;
}

long test_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
        switch(cmd) {
            case WR_VALUE:
                    if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) )
                    {
                            pr_err("Data Write : Err!\n");
                    }
                    pr_info("Value = %d\n", value);
                    break;
            case RD_VALUE:
                    if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                    {
                            pr_err("Data Read : Err!\n");
                    }
                    break;
            default:
                    pr_info("Default\n");
                    break;
    }
    return 0;
}


module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("tudo");
MODULE_DESCRIPTION("Simple linux driver (Statically allocating the Major and Minor number)");
MODULE_VERSION("1.0");
