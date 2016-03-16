/**
 * Operating Sytems 2013 - Assignment 2
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "utils.h"

#define PROMPT		"> "

void parse_error(const char *str, const int where)
{
	fprintf(stderr, "Parse error near %d: %s\n", where, str);
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
			ret = parse_command(root, 0, NULL, NULL);

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
