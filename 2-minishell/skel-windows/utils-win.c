/**
 * Operating Systems 2013 - Assignment 2
 *
 */

#include <windows.h>

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#define READ		0
#define WRITE		1

#define MAX_SIZE_ENVIRONMENT_VARIABLE 100

/**
 * Debug method, used by DIE macro.
 */
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

/**
 * Internal change-directory command.
 */
static bool shell_cd(word_t *dir)
{
	/* TODO execute cd */

	return 0;
}

/**
 * Internal exit/quit command.
 */
static int shell_exit(void)
{
	/* TODO execute exit/quit */

	return 0; /* TODO replace with actual exit code */
}

/**
 * Concatenate parts of the word to obtain the command
 */
static LPTSTR get_word(word_t *s)
{
	DWORD string_length = 0;
	DWORD substring_length = 0;

	LPTSTR string = NULL;
	CHAR substring[MAX_SIZE_ENVIRONMENT_VARIABLE];

	DWORD dwret;

	while (s != NULL) {
		strcpy(substring, s->string);

		if (s->expand == true) {
			dwret = GetEnvironmentVariable(
				substring,
				substring,
				MAX_SIZE_ENVIRONMENT_VARIABLE
			);
			if (!dwret)
				/* Environment Variable does not exist. */
				strcpy(substring, "");
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
static LPTSTR get_argv(simple_command_t *command)
{
	LPTSTR argv = NULL;
	LPTSTR substring = NULL;
	word_t *param;

	DWORD string_length = 0;
	DWORD substring_length = 0;

	argv = get_word(command->verb);
	assert(argv != NULL);

	string_length = strlen(argv);

	param = command->params;
	while (param != NULL) {
		substring = get_word(param);
		substring_length = strlen(substring);

		argv = realloc(argv, string_length + substring_length + 4);
		assert(argv != NULL);

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

/**
 * Parse and execute a simple command, by either creating a new processing or
 * internally process it.
 */
bool parse_simple(simple_command_t *s, int level, command_t *father, HANDLE *h)
{
	/* TODO sanity checks */

	/* TODO if builtin command, execute the command */

	/* TODO if variable assignment, execute the assignment and return
	 * the exit status
	 */

	/* TODO if external command:
	 *  1. set handles
	 *  2. redirect standard input / output / error
	 *  3. run command
	 *  4. get exit code
	 */

	return 0; /* TODO replace with actual exit status */
}

/**
 * Process two commands in parallel, by creating two children.
 */
static bool do_in_parallel(command_t *cmd1, command_t *cmd2, int level, command_t *father)
{
	/* TODO execute cmd1 and cmd2 simultaneously */

	return true; /* TODO replace with actual exit status */
}

/**
 * Run commands by creating an anonymous pipe (cmd1 | cmd2)
 */
static bool do_on_pipe(command_t *cmd1, command_t *cmd2, int level, command_t *father)
{
	/* TODO redirect the output of cmd1 to the input of cmd2 */

	return true; /* TODO replace with actual exit status */
}

/**
 * Parse and execute a command.
 */
int parse_command(command_t *c, int level, command_t *father, void *h)
{
	/* TODO sanity checks */

	if (c->op == OP_NONE) {
		/* TODO execute a simple command */

		return 0; /* TODO replace with actual exit code of command */
	}

	switch (c->op) {
	case OP_SEQUENTIAL:
		/* TODO execute the commands one after the other */
		break;

	case OP_PARALLEL:
		/* TODO execute the commands simultaneously */
		break;

	case OP_CONDITIONAL_NZERO:
		/* TODO execute the second command only if the first one
		 * returns non zero
		 */
		break;

	case OP_CONDITIONAL_ZERO:
		/* TODO execute the second command only if the first one
		 * returns zero
		 */
		break;

	case OP_PIPE:
		/* TODO redirect the output of the first command to the
		 * input of the second
		 */
		break;

	default:
		return SHELL_EXIT;
	}

	return 0; /* TODO replace with actual exit code of command */
}

/**
 * Readline from mini-shell.
 */
char *read_line()
{
	char *instr;
	char *chunk;
	char *ret;

	int instr_length;
	int chunk_length;

	int endline = 0;

	instr = NULL;
	instr_length = 0;

	chunk = calloc(CHUNK_SIZE, sizeof(char));
	if (chunk == NULL) {
		fprintf(stderr, ERR_ALLOCATION);
		exit(EXIT_FAILURE);
	}

	while (!endline) {
		ret = fgets(chunk, CHUNK_SIZE, stdin);
		if (ret == NULL)
			break;

		chunk_length = strlen(chunk);
		if (chunk[chunk_length - 1] == '\n') {
			chunk[chunk_length - 1] = 0;
			endline = 1;
		}

		instr = realloc(instr, instr_length + CHUNK_SIZE + 1);
		if (instr == NULL)
			break;

		memset(instr + instr_length, 0, CHUNK_SIZE);
		strcat(instr, chunk);
		instr_length += chunk_length;
	}

	free(chunk);

	return instr;
}

