// SPDX-License-Identifier: GPL-2.0
/*
 * hello2: calls print_hello() from hello1 hello_count times.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "hello1.h"

MODULE_AUTHOR("Ivan Romanenko");
MODULE_DESCRIPTION("Hello World kernel module: caller");
MODULE_LICENSE("GPL");

static unsigned int hello_count = 1;
module_param(hello_count, uint, 0644);
MODULE_PARM_DESC(hello_count, "Number of Hello World messages (uint)");

static int __init hello2_init(void)
{
	unsigned int i;

	if (hello_count > 10) {
		pr_err("hello2: count %u exceeds 10\n", hello_count);
		return -EINVAL;
	}
	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10))
		pr_warn("hello2: unusual count %u\n", hello_count);

	for (i = 0; i < hello_count; i++)
		print_hello();

	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2: Goodbye, world!\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
