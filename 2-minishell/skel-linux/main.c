/**
 * Operating Systems 2013-2017 - Assignment 2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "cmd.h"
#include "utils.h"

#define PROMPT             "> "

#define CHUNK_SIZE         100
#define ERR_ALLOCATION     "unable to allocate memory"


void parse_error(const char *str, const int where)
{
	fprintf(stderr, "Parse error near %d: %s\n", where, str);
}

/**
 * Readline from mini-shell.
 */
static char *read_line(void)
{
	char *instr;
	char *chunk;
	char *ret;

	int instr_length;
	int chunk_length;

	int endline = 0;

	chunk = calloc(CHUNK_SIZE, sizeof(char));
	if (chunk == NULL) {
		fprintf(stderr, ERR_ALLOCATION);
		exit(EXIT_FAILURE);
	}

	instr = NULL;
	instr_length = 0;

	while (!endline) {
		ret = fgets(chunk, CHUNK_SIZE, stdin);
		if (ret == NULL)
			break;

		chunk_length = strlen(chunk);
		if (chunk[chunk_length - 1] == '\n') {
			if (chunk_length > 1 && chunk[chunk_length - 2] == '\r')
				/* Windows */
				chunk[chunk_length - 2] = 0;
			else
				chunk[chunk_length - 1] = 0;
			endline = 1;
		}

		ret = instr;
		instr = realloc(instr, instr_length + CHUNK_SIZE + 1);
		if (instr == NULL) {
			free(ret);
			return instr;
		}

		memset(instr + instr_length, 0, CHUNK_SIZE);
		strcat(instr, chunk);
		instr_length += chunk_length;
	}

	free(chunk);

	return instr;
}

static void start_shell(void)
{
	char *line;
	command_t *root;

	int ret;

	for (;;) {
		printf(PROMPT);
		fflush(stdout);
		ret = 0;

		root = NULL;
		line = read_line();
		if (line == NULL)
			return;
		parse_line(line, &root);

		if (root != NULL)
			ret = parse_command(root, 0, NULL);

		free_parse_memory();
		free(line);

		if (ret == SHELL_EXIT)
			break;
	}
}

int main(void)
{
	start_shell();

	return EXIT_SUCCESS;
}
