/**
 * SO, 2010 - Lab #1, Introduction
 * Task #3, Windows
 * 
 * Building libraries
 */
 
 #ifdef DLL_IMPORTS
	#define DECLSEPC __declspec(dllexport)
#else
	#define DECLSEPC
#endif

DECLSEPC int get_min(int a, int b);

int get_min(int a, int b)
{
	if (a < b) 
		return a;
	else
		return b;
}
