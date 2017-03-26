/**
 * Operating Systems 2013-2017 - Assignment 2
 *
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <windows.h>
#include "parser.h"

VOID PrintLastError(const PCHAR message);

/* useful macro for handling error codes */
#define DIE(assertion, call_description) \
	do { \
		if (assertion) { \
			fprintf(stderr, "(%s, %s, %d): ", \
				__FILE__, __FUNCTION__, __LINE__); \
			PrintLastError(call_description); \
			exit(EXIT_FAILURE); \
		} \
	} while (0)

/**
 * Concatenate parts of the word to obtain the command
 */
LPTSTR get_word(word_t *s);

/**
 * Parse arguments in order to succesfully process them using CreateProcess
 */
LPTSTR get_argv(simple_command_t *command);

#endif /* _UTILS_H */
