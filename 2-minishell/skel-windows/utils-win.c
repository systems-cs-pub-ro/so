/**
 * Operating Systems 2013-2017 - Assignment 2
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
	DWORD string_length = 0;
	DWORD substring_length = 0;

	LPTSTR string = NULL;
	CHAR substring[MAX_SIZE_ENVIRONMENT_VARIABLE];

	DWORD dwret;

	while (s != NULL) {
		if (s->expand == true) {
			dwret = GetEnvironmentVariable(
				s->string,
				substring,
				MAX_SIZE_ENVIRONMENT_VARIABLE
			);
			if (!dwret)
				/* Environment Variable does not exist. */
				strcpy(substring, "");

		} else {
			strcpy(substring, s->string);
		}

		substring_length = strlen(substring);

		string = realloc(string, string_length + substring_length + 1);
		if (string == NULL)
			return NULL;

		memset(string + string_length, 0, substring_length + 1);

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
	LPTSTR substring = NULL;
	word_t *param;

	DWORD string_length = 0;
	DWORD substring_length = 0;

	argv = get_word(command->verb);
	DIE(argv == NULL, "Error retrieving word.");

	string_length = strlen(argv);

	param = command->params;
	while (param != NULL) {
		substring = get_word(param);
		substring_length = strlen(substring);

		argv = realloc(argv, string_length + substring_length + 4);
		DIE(argv == NULL, "Error reallocating argv.");

		strcat(argv, " ");

		/* Surround parameters with ' ' */
		strcat(argv, "'");
		strcat(argv, substring);
		strcat(argv, "'");

		string_length += substring_length + 3;
		param = param->next_word;

		free(substring);
	}

	return argv;
}


