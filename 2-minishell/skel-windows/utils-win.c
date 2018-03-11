/**
 * Operating Systems 2013-2018 - Assignment 2
 *
 */

#include <windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#define MAX_SIZE_ENVIRONMENT_VARIABLE 512

/**
 * Debug method, used by DIE macro.
 */
VOID PrintLastError(const PCHAR message)
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

/**
 * Concatenate parts of the word to obtain the command
 */
LPTSTR get_word(word_t *s)
{
	LPTSTR string = NULL;
	DWORD string_length = 0;

	LPCTSTR substring;
	DWORD substring_length = 0;

	CHAR envval[MAX_SIZE_ENVIRONMENT_VARIABLE];

	DWORD dwret;

	while (s != NULL) {
		if (s->expand == true) {
			dwret = GetEnvironmentVariable(
				s->string,
				envval,
				MAX_SIZE_ENVIRONMENT_VARIABLE
			);
			if (dwret)
				substring = envval;
			else
				/* Environment Variable does not exist. */
				substring = "";

		} else
			substring = s->string;

		substring_length = strlen(substring);

		string = realloc(string, string_length + substring_length + 1);
		DIE(string == NULL, "Error allocating word string.");

		string[string_length] = '\0';
		strcat(string, substring);

		string_length += substring_length;

		s = s->next_part;
	}

	return string;
}

/**
 * Parse arguments in order to succesfully process them using CreateProcess
 */
LPTSTR get_argv(simple_command_t *command)
{
	LPTSTR argv = NULL;
	DWORD argv_length = 0;

	LPTSTR substring = NULL;
	DWORD substring_length = 0;

	word_t *param;

	argv = get_word(command->verb);
	DIE(argv == NULL, "Error retrieving word.");

	argv_length = strlen(argv);

	param = command->params;
	while (param != NULL) {
		substring = get_word(param);
		DIE(substring == NULL, "Error retrieving word.");

		substring_length = strlen(substring);

		/* 3 == space + 2 * single quotes; 1 == '\0' */
		argv = realloc(argv, argv_length + substring_length + 3 + 1);
		DIE(argv == NULL, "Error reallocating argv.");

		argv[argv_length++] = ' ';

		/* Surround parameters with single quotes */
		argv[argv_length++] = '\'';

		memcpy(argv + argv_length, substring, substring_length);
		argv_length += substring_length;

		argv[argv_length++] = '\'';

		free(substring);

		param = param->next_word;
	}

	argv[argv_length] = '\0';

	return argv;
}


