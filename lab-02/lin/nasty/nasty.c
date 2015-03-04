/**
 * SO, 2011
 * Lab #2, Procese
 *
 * nasty.c - Helper driver implementation
 * Helper driver, providing a '/dev/nasty' device file. Reading
 * this file returns a random number betwen 1 and MAX_CHUNK_SIZE
 * characters from a given text.
 *
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/random.h>
#include <asm/uaccess.h>

#define NASTY_DRIVER_VERSION "1.0.0"
#define NASTY_DRIVER_NAME    "nasty"
#define FILE_MAX_SIZE  	     4096
#define MAX_CHUNK_SIZE	     16

char content[FILE_MAX_SIZE] = "\n\
	I have a dog.\n\
	His name is Fred.\n\
	He won't play fetch.\n\
	He won't play dead.\n\n\
	He won't shake hands\n\
	or sit or stay\n\
	or bark or beg\n\
	or run and play.\n\n\
	He won't roll over,\n\
	shake or crawl.\n\
	In fact, he won't\n\
	do tricks at all.\n\n\
	When folks ask why\n\
	I tell them that's\n\
	because my dog\n\
	was raised by cats.\n";

int crt_size = FILE_MAX_SIZE; /* file's current size */

static int nasty_open(struct inode *inode, struct file *file)
{
	if (file->f_flags & O_TRUNC)
		crt_size = 0;

	pr_debug("@open: crt_size %d\n", crt_size);
	return 0;
}

static int nasty_close(struct inode *inode, struct file *file)
{
	pr_debug("@close: crt_size %d\n", crt_size);
	return 0;
}

static ssize_t nasty_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) 
{
	int rcount, ret; /* random bytes to read */

	/* end of file */
	if (*ppos >= crt_size)
		return 0;

	/*  make sure that we read at least 1 character */
	rcount = 1 + random32() % MAX_CHUNK_SIZE; 
	
	/* ... and don't cross the borders */
	if (rcount > count)
		rcount = count;
	if (*ppos + rcount > crt_size)
		rcount = crt_size - *ppos;
	ret = copy_to_user(buf, &content[*ppos], rcount);
	if (ret != 0) {
		pr_warning("@read: copy to user failed\n");
		return -EFAULT;
	}	
	pr_debug("@read: requested %d, received %d, current pos %lld\n", 
			count, rcount, *ppos);
	*ppos += rcount;

	return rcount;
}


static ssize_t nasty_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) 
{
	int rcount, ret; /* random bytes to write */

	/* 0 in -> 0 out, no strings attached  */
	if (count == 0) 
		return 0;

	/* end of story */
	if (*ppos >= FILE_MAX_SIZE)
		return -ENOSPC;

	/*  make sure that we write at least 1 character */
	rcount = 1 + random32() % MAX_CHUNK_SIZE; 
	
	/* ... and don't cross the borders */
	if (rcount > count)
		rcount = count;
	if (*ppos + rcount > FILE_MAX_SIZE)
		rcount = FILE_MAX_SIZE - *ppos;
	ret = copy_from_user(&content[*ppos], buf, rcount);
	if (ret != 0) {
		pr_warning("@write: copy to user failead\n");
		return -EFAULT;
	}	
	pr_debug("@write: requested %d, written %d, current pos %lld\n", 
			count, rcount, *ppos);
	*ppos += rcount;
	crt_size = max(crt_size, (int)*ppos);
	
	return rcount;
}

static struct file_operations nasty_fops = {
	.owner 		= THIS_MODULE,
	.open  		= nasty_open,
	.read  		= nasty_read,
	.write 		= nasty_write,
	.release 	= nasty_close
};

static struct miscdevice nasty_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = NASTY_DRIVER_NAME,
	.fops  = &nasty_fops
};


static int __init nasty_init(void)
{
	int ret;
	
	ret = misc_register(&nasty_device);
	if (ret < 0) {
		pr_warning("Misc register failed\n");
		return ret;
	}
	
	pr_info("Module %s version %s loaded!\n", 
				NASTY_DRIVER_NAME, NASTY_DRIVER_VERSION);
	return 0;
}


static void __exit nasty_exit(void)
{
	misc_deregister(&nasty_device);
	pr_info("Module %s version %s unloaded!\n", 
				NASTY_DRIVER_NAME, NASTY_DRIVER_VERSION);
}

module_init(nasty_init);
module_exit(nasty_exit);

MODULE_AUTHOR("OS course team, <elf.cs.pub.ro/so>");
MODULE_DESCRIPTION("Simple char driver creating a nasty read/write device file");
MODULE_VERSION(NASTY_DRIVER_VERSION);
MODULE_LICENSE("GPL");


