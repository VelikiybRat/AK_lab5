#define DEBUG
// SPDX-License-Identifier: GPL-2.0
/*
 * hello1: keeps the list of timestamps and exports print_hello().
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Ivan Romanenko");
MODULE_DESCRIPTION("Hello World kernel module: list and print");
MODULE_LICENSE("GPL");

struct hello_event {
	struct list_head list;
	ktime_t time_stamp;
	ktime_t duration;
};

static LIST_HEAD(hello_list);

void print_hello(void)
{
	struct hello_event *ev;
	ktime_t start, end;

	ev = kmalloc(sizeof(*ev), GFP_KERNEL);
	if (!ev)
		return;

	start = ktime_get();
	pr_info("Hello, world!\n");
	end = ktime_get();

	ev->time_stamp = start;
	ev->duration = ktime_sub(end, start);
	list_add_tail(&ev->list, &hello_list);
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1: loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_event *ev, *tmp;

	pr_debug("hello1: exit started\n");
	list_for_each_entry_safe(ev, tmp, &hello_list, list) {
		pr_debug("hello1: print took %lld ns\n",
			 ktime_to_ns(ev->duration));
		list_del(&ev->list);
		kfree(ev);
	}
	pr_debug("hello1: exit finished\n");
	pr_info("hello1: unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
