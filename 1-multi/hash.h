
#ifndef HASH_H_
#define HASH_H_

#ifdef WIN
#ifdef DLL_IMPORTS
#define DLL_DECLSPEC __declspec(dllimport)
#else
#define DLL_DECLSPEC __declspec(dllexport)
#endif

DLL_DECLSPEC unsigned int hash(const char *str, unsigned int hash_length);

#else

unsigned int hash(const char *str, unsigned int hash_length);

#endif

#endif

