#include <linux/init.h>
#include <linux/module.h>		//TODO remove includes that you do not use
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/sched.h>

struct task_struct *task;

int simple_init(void)
{
	printk(KERN_INFO "Loading Task List Module\n\n");
	
	printk(KERN_INFO "STATE\t\tPID\t\tNAME\n");
	for_each_process(task) {							// iterates through each task
		printk("%ld \t\t %d \t\t %s\n",task->state, task->pid, task->comm);	// output some task properties
	} 

	return 0;
}

void simple_exit(void)
{
	printk(KERN_INFO "Removing Task List Module\n");
}

module_init(simple_init);	// macros to attach functions to modules initialization and removal
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
