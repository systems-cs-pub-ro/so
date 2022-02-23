/**
 * SO
 * Lab #3
 *
 * Task #3, Linux
 *
 * Implementing a minimal shell
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils.h"

#define MAX_LINE_SIZE		256
#define MAX_ARGS		8

#define ERROR			0
#define SIMPLE			1
#define REDIRECT		2
#define PIPE			3
#define SET_VAR			4
#define EXIT_CMD		5

static char *verb;
static char **args;
static char *stdin_file;
static char *stdout_file;
static char *stderr_file;

static char *value;
static char *var;

static int type;

static int parse_line(char *line);
static void alloc_mem(void);

/**
 * @filedes  - file descriptor to be redirected
 * @filename - filename used for redirection
 */
static void do_redirect(int filedes, const char *filename)
{
	int ret;
	int fd;

	/**
	 * TODO - Redirect filedes into file filename
	 */
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	DIE(fd < 0, "open");

	ret = dup2(fd, filedes);
	DIE(ret < 0, "dup2");

	close(fd);
}


static void set_var(const char *var, const char *value)
{
	/**
	 * TODO - Set the environment variable
	 */
	setenv(var, value, 1);
}

static char *expand(const char *key)
{
	/**
	 * TODO - Return the value of environment variable
	 */
	return getenv(key);
}

/**
 *  @args - array that contains a simple command with parameters
 */
static void simple_cmd(char **args)
{
	pid_t pid;
	pid_t wait_ret;
	int status;

	/**
	 * TODO - Create a process to execute the command
	 */
	pid = fork();
	switch (pid) {
	case -1:	/* error */
		perror("fork");
		exit(EXIT_FAILURE);

	case 0:		/* child process */
		if (stdout_file != NULL)
			do_redirect(STDOUT_FILENO, stdout_file);

		execvp(args[0], (char *const *) args);

		fprintf(stderr, "Execution failed for '%s'\n", args[0]);
		fflush(stdout);

		exit(EXIT_FAILURE);

	default:	/* parent process */
		wait_ret = waitpid(pid, &status, 0);
		DIE(wait_ret < 0, "waitpid");
		if (WIFEXITED(status))
			printf("Child process (pid %d) terminated normally, "
					"with exit code %d\n",
					pid, WEXITSTATUS(status));
	}
}

int main(void)
{
	char line[MAX_LINE_SIZE];

	alloc_mem();

	while (1) {
		printf("> ");
		fflush(stdout);

		memset(line, 0, MAX_LINE_SIZE);

		if (fgets(line, sizeof(line), stdin) == NULL)
			exit(EXIT_SUCCESS);

		type = parse_line(line);

		switch (type) {
		case EXIT_CMD:
			exit(EXIT_SUCCESS);

		case SET_VAR:
			set_var(var, value);
			break;

		case SIMPLE:
			simple_cmd(args);
			break;
		}
	}

	return 0;
}

static void alloc_mem(void)
{
	args = malloc(MAX_ARGS * sizeof(char *));
}

static int parse_line(char *line)
{
	int ret = SIMPLE;
	int idx = 0;
	char *token;
	char *delim = "=\n";;

	stdin_file = NULL;
	stdout_file = NULL;
	stderr_file = NULL;

	/* Check for exit */
	if (strncmp("exit", line, strlen("exit")) == 0)
		return EXIT_CMD;

	/* var = value */
	if (strchr(line, '=') != 0) {
		token = strtok(line, delim);

		if (token == NULL)
			return ERROR;

		var = strdup(token);

		token = strtok(NULL, delim);
		if (token == NULL)
			return ERROR;

		value = strdup(token);
		return SET_VAR;
	}

	/* Normal command */
	delim = " \t\n";
	token = strtok(line, delim);

	if (token == NULL)
		return ERROR;

	verb = strdup(token);

	/* Copy args */
	idx = 0;
	while (token != NULL) {
		if (token[0] == '$')
			token = expand(token+1);

		if (token == NULL) {
			printf(" Expansion failed\n");
			return ERROR;
		}

		if (strncmp(token, ">", strlen(">")) == 0) {
			if (strlen(token) > strlen(">")) {
				token++;
				stdout_file = strdup(token);
			} else {
				token = strtok(NULL, delim);
				stdout_file = strdup(token);
			}
		} else {
			args[idx++] = strdup(token);
		}

		token = strtok(NULL, delim);
	}

	args[idx++] = NULL;
	return ret;
}
