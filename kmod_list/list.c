#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/list.h>

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;			// this allows each node to know about the list it's in
};

static LIST_HEAD(birthday_list);	// macro to create and init the list birthday_list
struct birthday *ptr, *next;
int i = 0;

int simple_init(void)
{
	printk(KERN_INFO "Loading List Module\n");
	
	for(i = 0; i < 5; i++){
		struct birthday *person;
		person = kmalloc(sizeof(*person), GFP_KERNEL);	// allocate memory for the entry struct person
		person->day   = 1 + i;
		person->month = 1 + i;
		person->year  = 1990 + i;
		list_add_tail(&person->list, &birthday_list);	// add each entry to the end of the list
	}

	list_for_each_entry(ptr, &birthday_list, list) {
		/*on each iteration ptr points to the next birthday struct */
		printk(KERN_INFO "checking entry: %i %i %i", ptr->month, ptr->day, ptr->year);
	} 

	return 0;
}

void simple_exit(void)
{
	printk(KERN_INFO "Removing List Module\n");

	list_for_each_entry_safe(ptr, next, &birthday_list, list) {
		/*on each iteration ptr points to the next birthday struct */
		printk(KERN_INFO "removing entry: %i %i %i", ptr->month, ptr->day, ptr->year);
		list_del(&(ptr->list));		// remove list entry from list
		kfree(ptr);					// free allocated memory
	} 
}

module_init(simple_init);	// macros to attach functions to modules initialization and removal
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
