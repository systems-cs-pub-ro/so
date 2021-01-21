#include <stdio.h>

static unsigned long f1_addr, f2_addr;

static void f2(void)
{
	int x;
	printf("In f2; stack variable is at %p\n", &x);
	f2_addr = (unsigned long) &x;
}

static void f1(void)
{
	int x;
	printf("In f1; stack variable is at %p\n", &x);
	f1_addr = (unsigned long) &x;

	f2();
}

int main(void)
{
	f1();

	/*
	 * f2 is called from f1, so f2's stack frame is 'higher' on the
	 * stack.
	 */
	if (f1_addr > f2_addr)
		printf("Stack grows down.\n");
	else
		printf("Stack grows up.\n");

	return 0;
}
