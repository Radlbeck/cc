#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;		// this allows each node to know about the list it's in
}

list_head birthday_list;
static LIST_HEAD(birthday_list);

int simple_init(void)
{
	printk(KERN_INFO "Loading List Module\n");
	
	struct birthday *person;
	person = kmalloc(sizeof(*person), GF_KERNEL);
	person->day   = 2;
	person->month = 2;
	person->year  = 1992;
	INIT_LIST_HEAD(&person->list);

	return 0;
}

void simple_exit(void)
{
	printk(KERN_INFO "Removing List Module\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
