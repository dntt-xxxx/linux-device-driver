/***************************************************************************//**
*  \file       driver.c
*
*  \details    Simple linux driver (Statically allocating the Major and Minor number)
*
*  \author     EmbeTronicX
*
* *******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include <linux/fs.h>

//creating the dev with our custom major and minor number
dev_t dev = MKDEV(233, 2);
dev_t dyn_dev;


/*
** Module Init function
*/
static int __init hello_world_init(void)
{
    register_chrdev_region(dev, 8, "mm-static-aloc-test");
    alloc_chrdev_region(&dyn_dev, 0, 1, "mm-dynm-aloc-test");

    printk(KERN_INFO "STATIC: Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
    printk(KERN_INFO "DYNAMIC: Major = %d Minor = %d \n",MAJOR(dyn_dev), MINOR(dyn_dev));
    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;
}

/*
** Module exit function
*/
static void __exit hello_world_exit(void)
{
    unregister_chrdev_region(dev, 8);
    unregister_chrdev_region(dyn_dev, 1);
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("tudo");
MODULE_DESCRIPTION("Simple linux driver (Statically allocating the Major and Minor number)");
MODULE_VERSION("1.0");
