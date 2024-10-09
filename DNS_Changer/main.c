#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "dns_settings"
#define DNS_MAX_LEN 256

// Default DNS
static char dns_settings[DNS_MAX_LEN] = "8.8.8.8";

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DNS Settings Module");
MODULE_AUTHOR("Mehdi");
MODULE_VERSION("1.0.0");

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[DNS_MAX_LEN];
    static int completed = 0;

    if (completed)
    {
        completed = 0;
        return 0;
    }

    completed = 1;
    rv = sprintf(buffer, "%s\n", dns_settings);
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
    copy_from_user(dns_settings, usr_buf, count);
    dns_settings[count] = '\0'; // Null-terminate the string

    return count;
}

static struct proc_ops proc_ops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};

static int __init dns_module_init(void)
{
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "DNS module loaded with default DNS: %s\n", dns_settings);
    return 0;
}

static void __exit dns_module_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "DNS module unloaded.\n");
}

module_init(dns_module_init);
module_exit(dns_module_exit);
