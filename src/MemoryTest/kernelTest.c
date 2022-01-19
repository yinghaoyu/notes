#include <linux/module.h>
static unsigned long addr = 0;
module_param(addr, long, 0644);
static int test_init(void)
{
	strcpy(phys_to_virt(addr), (char *)"rain flooding water will not get fat!");
	return 0;
}
static void test_exit(void)
{
}
module_init(test_init);
module_exit(test_exit);
