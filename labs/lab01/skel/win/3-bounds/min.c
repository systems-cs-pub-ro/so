/**
 * SO
 * Lab #1, Introduction
 *
 * Task #3, Windows
 *
 * Building libraries
 */

__declspec(dllexport) int get_min(int a, int b);

int get_min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}
