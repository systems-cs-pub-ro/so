#include <windows.h>
#include <stdio.h>

void f(int a, int b)
{
	int c;

	c = a + b;

	return c;
}

int main(void)
{

	*bug = f(1, 2);

	printf("bug = %d\n", bug);

	return 0;
}
