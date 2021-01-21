/**
 * SO, 2016
 * Windows
 *
 * Useful structures/macros
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#ifndef WIN_UTILS_H_
#define WIN_UTILS_H_	1

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) < (b) ? (b) : (a))

/* useful macro for handling error codes */
#define DIE(assertion, call_description)			\
	do {							\
		if (assertion) {				\
			fprintf(stderr, "(%s, %s, %d): ",	\
			__FILE__, __FUNCTION__, __LINE__);	\
			PrintLastError(call_description);	\
			exit(EXIT_FAILURE);			\
		}						\
	} while (0)

static VOID PrintLastError(const PCHAR message)
{
	CHAR errBuff[1024];

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
		NULL,
		GetLastError(),
		0,
		errBuff,
		sizeof(errBuff) - 1,
		NULL);

	fprintf(stderr, "%s: %s\n", message, errBuff);
}

#define DEBUG
#ifdef DEBUG
	#define dbg(format, ...) fprintf(stderr, format, __VA_ARGS__)
#else
	#define dbg(format, ...)
#endif

#endif
