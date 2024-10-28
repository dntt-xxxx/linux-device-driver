/***************************************************************************//**
*  \file       driver.c
*
*  \details    Simple hello world driver
*
*  \author     EmbeTronicX
*
* *******************************************************************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
 
/*
** Module Init function
*/
struct cus_type {
    char name[16];
    unsigned int age;
    int magic_num;
};

struct cus_type dotu ={
    .name = "DOTU",
    .age = 24,
    .magic_num =1603
};


int test_int_value, test_array_value[4]={1,2,3,4};
char test_charp_value[100];

// module_param(test_int_value, int, S_IWUSR|S_IRUSR);
// module_param(test_charp_value, charp, S_IWUSR|S_IRUSR);
// module_param_array(test_array_value, int,NULL, S_IWUSR|S_IRUSR);


/*----------------------Module_param_cb()--------------------------------*/
// for int 
#if 0
int param_int_set(const char *val, const struct kernel_param *kp)
{
        int res = param_set_int(val, kp); // Use helper for write variable
        if(res==0) {
                printk(KERN_INFO "Call back set function called...\n");
                printk(KERN_INFO "New value of test_int_value = %d\n", test_int_value);
                return 0;
        }
        return -1;
}
int param_int_get(char *buffer, const struct kernel_param *kp)
{
    return sprintf(buffer, "%d\n", *((int *)kp->arg));
}

const struct kernel_param_ops my_param_ops = 
{
        .set = &param_int_set, // Use our setter ...
        .get = &param_int_get, // .. and standard getter
};

module_param_cb(test_int_value, &my_param_ops, &test_int_value, S_IRUGO|S_IWUSR );
#endif

#if 0
int param_char_set(const char *val, const struct kernel_param *kp)
{
        strncpy((char *)kp->arg, val, sizeof(test_charp_value) - 1);
        ((char *)kp->arg)[sizeof(test_charp_value)] = '\0'; // Null terminate to prevent overflow
        printk(KERN_INFO "Call back set function called...\n");
        printk(KERN_INFO "New value of test_int_value = %s\n",(char*) kp->arg);
        return 0;
}
int param_char_get(char *buffer, const struct kernel_param *kp)
{
    return sprintf(buffer, "%s\n", ((char *)kp->arg));
}

const struct kernel_param_ops my_param_ops = 
{
        .set = &param_char_set, // Use our setter ...
        .get = &param_char_get, // .. and standard getter
};

module_param_cb(test_charp_value, &my_param_ops, &test_charp_value, S_IRUGO|S_IWUSR );
#endif


#if 1 //custome type
int param_cus_set(const char *val, const struct kernel_param *kp)
{

        char name[16];
        unsigned int age;
        int magic_number;
        struct cus_type *tmp;
        tmp = ((struct cus_type*) (kp->arg));

        printk(KERN_INFO "Call back set function called...\n");

        int ret = sscanf(val, "%s %d %d", name, &age,&magic_number);
        if (ret != 3) {
            printk(KERN_WARNING "Invalid format for config parameter. Expected: <name> <age> <magic_number>\n");
            return -EINVAL;
        }
        strncpy(tmp->name, name, sizeof(name) - 1);

        tmp->age=age;
        tmp->magic_num = magic_number;

        printk(KERN_INFO "New value of test_int_value = %s\n",(char*) kp->arg);
        return 0;
}
int param_cus_get(char *buffer, const struct kernel_param *kp)
{
    struct cus_type *tmp;
    tmp = ((struct cus_type*) (kp->arg));
    return sprintf(buffer, "%s %d %d\n", tmp->name,tmp->age,tmp->magic_num);
}

const struct kernel_param_ops my_param_ops = 
{
        .set = &param_cus_set, // Use our setter ...
        .get = &param_cus_get, // .. and standard getter
};

module_param_cb(dotu, &my_param_ops, &dotu, S_IRUGO|S_IWUSR );
#endif



static int __init hello_world_init(void)
{
    int i=0;
    printk(KERN_INFO "Test paramater passing module\n");
    printk(KERN_DEBUG "test_int_value = %d\n",test_int_value);
    printk(KERN_DEBUG "test_charp_value = %s\n",test_charp_value);
    printk(KERN_DEBUG "test_array_value = ");
    for ( i = 0; i < sizeof(test_array_value)/sizeof(int); i++)
    {
        printk(KERN_DEBUG "%d ",test_array_value[i]);
    }
    printk(KERN_DEBUG "\n");

    printk(KERN_DEBUG "Kernel Module Inserted Successfully...\n");
    return 0;
}

/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    printk(KERN_DEBUG "Test paramater passing module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("tester");
MODULE_DESCRIPTION("test");
MODULE_VERSION("2:1.0");
