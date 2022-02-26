/**
 * SO
 * Lab #1, Introduction
 *
 * Task #3, Windows
 *
 * Building libraries
 */

#ifdef DLL_IMPORTS
	#define DECLSPEC __declspec(dllexport)
#else
	#define DECLSPEC
#endif

DECLSPEC int get_max(int a, int b);

int get_max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}
