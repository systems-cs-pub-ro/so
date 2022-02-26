/**
 * SO
 * Lab #1, Introduction
 *
 * Task #3, Windows
 *
 * Building libraries
 */

#include <windows.h>
#include <stdio.h>

#define FIRST_NUMBER 39
#define SECOND_NUMBER 42

 #ifdef DLL_IMPORTS
	#define DECLSPEC __declspec(dllimport)
#else
	#define DECLSPEC
#endif

DECLSPEC int get_min(int a, int b);
DECLSPEC int get_max(int a, int b);

int main(void)
{
	int min, max;

	min = get_min(FIRST_NUMBER, SECOND_NUMBER);
	printf("min(%d, %d) = %d\n", FIRST_NUMBER, SECOND_NUMBER, min);

	max = get_max(FIRST_NUMBER, SECOND_NUMBER);
	printf("max(%d, %d) = %d\n", FIRST_NUMBER, SECOND_NUMBER, max);

	return 0;
}
