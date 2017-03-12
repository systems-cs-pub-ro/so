/**
 * Operating Systems 2013-2017 - Assignment 2
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

/**
 * Concatenate parts of the word to obtain the command
 */
char *get_word(word_t *s)
{
	int string_length = 0;
	int substring_length = 0;

	char *string = NULL;
	char *substring = NULL;

	while (s != NULL) {
		if (s->expand == true) {
			substring = getenv(s->string);

			/* prevents strlen from failing */
			if (substring == NULL)
				substring = "";

		} else {
			substring = strdup(s->string);

			if (substring == NULL) {
				if (string)
					free(string);
				return NULL;
			}
		}

		substring_length = strlen(substring);

		string = realloc(string, string_length + substring_length + 1);
		if (string == NULL) {
			if (s->expand == false)
				free(substring);
			return NULL;
		}

		memset(string + string_length, 0, substring_length + 1);

		strcat(string, substring);
		string_length += substring_length;

		if (s->expand == false)
			free(substring);

		s = s->next_part;
	}

	return string;
}

/**
 * Concatenate command arguments in a NULL terminated list in order to pass
 * them directly to execv.
 */
char **get_argv(simple_command_t *command, int *size)
{
	char **argv;
	word_t *param;

	int argc = 0;

	argv = calloc(argc + 1, sizeof(char *));
	DIE(argv == NULL, "Error allocating argv.");

	argv[argc] = get_word(command->verb);
	DIE(argv[argc] == NULL, "Error retrieving word.");

	argc++;

	param = command->params;
	while (param != NULL) {
		argv = realloc(argv, (argc + 1) * sizeof(char *));
		DIE(argv == NULL, "Error reallocating argv.");

		argv[argc] = get_word(param);
		DIE(argv[argc] == NULL, "Error retrieving word.");

		param = param->next_word;
		argc++;
	}

	argv = realloc(argv, (argc + 1) * sizeof(char *));
	DIE(argv == NULL, "Error reallocating argv.");

	argv[argc] = NULL;
	*size = argc;

	return argv;
}


