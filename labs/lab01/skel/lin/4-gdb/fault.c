/**
 * SO, 2016
 * Lab #1, Introduction
 *
 * Task #4, Linux
 *
 * Debugging with gdb
 */


#include <stdio.h>

#define ARRAY_MAX_SIZE 8192

void print_array(int v[], int n)
{
	int i;

	for (i = 0; i < ARRAY_MAX_SIZE; i++) {
		printf("%d ", v[i]);
		if (i % 11 == 0)
			printf("\n");
	}
}

int inv_sum(int v[], int n)
{
	int i, isum = 0;

	for (i = 0; i < n; i++)
		isum += 1/v[i];
	return isum;
}

void show_inv(int v[], int n)
{
	printf("Inverses sum for: [ ");

	print_array(v, n);

	printf("] is: %d\n", inv_sum(v, n));
}

int main(void)
{
	int v[] = { 4, 3, 0, 3, 2};

	show_inv(v, 5);

	return 0;
}
