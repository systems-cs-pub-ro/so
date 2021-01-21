/**
 * SO, 2016
 * Lab #5, Gestiunea Memoriei
 *
 * Task #1, Windows
 *
 * Test source for malloc wrapper
 */


/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>

#include "xmalloc.h"
#include "utils.h"


static int ***tensor_alloc(int l, int m, int n)
{
	int i, j;
	int ***t;
	/* TODO */

	return t;
}

static void tensor_free(int ***t, int l, int m)
{
	int i, j;

	/* TODO */

}

static void tensor_init(int ***t, int l, int m, int n)
{
	int i, j, k;

	for (i = 0; i < l; i++)
		for (j = 0; j < m; j++)
			for (k = 0; k < n; k++)
				t[i][j][k] = i + j + k;
}

static void tensor_print(int ***t, int l, int m, int n)
{
	int i, j, k;

	for (i = 0; i < l; i++)
		for (j = 0; j < m; j++)
			for (k = 0; k < n; k++)
				printf("t[%d][%d][%d] = %d\n", i, j, k,
						t[i][j][k]);
}

int main(void)
{
	int ***t;

	t = tensor_alloc(2, 3, 4);
	tensor_init(t, 2, 3, 4);
	tensor_print(t, 2, 3, 4);
	tensor_free(t, 2, 3);

	return 0;
}
