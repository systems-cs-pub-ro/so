/**
 * Operating Sytems 2013-2017 - Assignment 2
 *
 * TODO Name, Group
 *
 */

#ifndef _CMD_H
#define _CMD_H

#include "parser.h"

#define SHELL_EXIT -100

/**
 * Parse and execute a command.
 */
int parse_command(command_t *cmd, int level, command_t *father);

#endif /* _CMD_H */
