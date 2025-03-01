#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

//Header to access memory stats
#include <linux/mm.h>

//Headers to write /proc file
#include <linux/proc_fs.h>
#include <linux/seq_file.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prasad");
MODULE_DESCRIPTION("Number of pagefaults");

static int num_pagefaults_show(struct seq_file *m, void *v) {
	unsigned long p=(unsigned long)this_cpu_read(vm_event_states.event[PGFAULT]);	//Read page faults count
	seq_printf(m, "%lu\n",p);	//Print to file
  return 0;
}

static int num_pagefaults_open(struct inode *inode, struct  file *file) {
  return single_open(file, num_pagefaults_show, NULL);
}

static const struct file_operations num_pagefaults_fops = {
  .owner = THIS_MODULE,
  .open = num_pagefaults_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

static int __init num_pagefaults_init(void)
{
	proc_create("num_pagefaults", 0, NULL, &num_pagefaults_fops);	//Creates /proc entry
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit num_pagefaults_exit(void)
{
    remove_proc_entry("num_pagefaults", NULL);	//Removes /proc entry
}

module_init(num_pagefaults_init);
module_exit(num_pagefaults_exit);

MODULE_LICENSE("GPL");

