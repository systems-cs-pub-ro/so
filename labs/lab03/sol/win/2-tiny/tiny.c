/**
 * SO
 * Lab #3
 *
 * Task #1, Windows
 *
 * Implementing a minimal shell
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define MAX_LINE_SIZE	256
#define MAX_CMD_SIZE	32
#define MAX_ARGS		8

#define	ERROR			0
#define SIMPLE			1
#define PIPE			2
#define SET_VAR			3
#define EXIT_CMD		4

static PCHAR command1, command2;
static PCSTR stdinFilename, stdoutFilename, stderrFilename;
static PCHAR value, var;
static CHAR varBuf[32];
static INT type;

static INT ParseLine(PCHAR line);
static INT ParseSimpleCommand(PCHAR line, PCHAR *command);
static VOID AllocateMemory(VOID);
static VOID Cleanup(VOID);
static HANDLE MyOpenFile(PCSTR filename);

/**
 * @psi		- STATRTUPINFO of the child process
 * @hFile	- file handle for redirect
 * @opt		- redirect option is one of the following
 *		 STD_INPUT_HANDLE,STD_OUTPUT_HANDLE, STD_ERROR_HANDLE
 */
static VOID RedirectHandle(STARTUPINFO *psi, HANDLE hFile, INT opt)
{
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	/* TODO 1 - Redirect */
	ZeroMemory(psi, sizeof(*psi));
	psi->cb = sizeof(*psi);

	psi->hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	psi->hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	psi->hStdError = GetStdHandle(STD_ERROR_HANDLE);

	psi->dwFlags |= STARTF_USESTDHANDLES;

	switch (opt) {
	case STD_INPUT_HANDLE:
		psi->hStdInput = hFile;
		break;
	case STD_OUTPUT_HANDLE:
		psi->hStdOutput = hFile;
		break;
	case STD_ERROR_HANDLE:
		psi->hStdError = hFile;
		break;
	}
}

static PCHAR ExpandVariable(PCSTR key)
{
	DWORD dwRes;

	memset(varBuf, 0, MAX_CMD_SIZE);
	dwRes = GetEnvironmentVariable(key, varBuf, sizeof(varBuf));
	DIE(!dwRes, "GetEnvironmentVariable");

	return varBuf;
}

static VOID CloseProcess(LPPROCESS_INFORMATION lppi)
{
	CloseHandle(lppi->hThread);
	CloseHandle(lppi->hProcess);
}

static VOID RunSimpleCommand(PCHAR command)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD dwRes;
	BOOL bRes;
	HANDLE hFile;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (stdoutFilename != NULL) {
		hFile = MyOpenFile(stdoutFilename);
		RedirectHandle(&si, hFile, STD_OUTPUT_HANDLE);
	}
	
	bRes = CreateProcess(
			NULL,
			command,
			NULL,
			NULL,
			TRUE,
			0,
			NULL,
			NULL,
			&si,
			&pi);
	DIE(!bRes, "Create Process");

	dwRes = WaitForSingleObject(pi.hProcess, INFINITE);
	DIE(dwRes == WAIT_FAILED, "WaitForSingleObject");

	CloseProcess(&pi);

	if (stdoutFilename != NULL)
		CloseHandle(hFile); 
}

/**
 * Purpose: launch a command like' command1 | command2 '
 *   @command1 - first command string
 *   @command2 - second command string
 */
static VOID PipeCommands(PCHAR command1, PCHAR command2)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;
	
	STARTUPINFO si1, si2;
	PROCESS_INFORMATION pi1, pi2;
	DWORD dwRes;
	BOOL bRes;

	/** 
	 * TODO 2 - Init security attributes and process info
	 */
	ZeroMemory(&sa, sizeof(sa));
	sa.bInheritHandle = TRUE;

	ZeroMemory(&pi1, sizeof(pi1));
	ZeroMemory(&pi2, sizeof(pi2));

	/**
	 * TODO 2 - Create pipe
	 */
	bRes = CreatePipe(&hRead, &hWrite, &sa, 0);
	DIE(bRes == FALSE, "CreatePipe");

	/**
	 * TODO 2 - Launch command1 with stdout redirected into the pipe
	 */
	RedirectHandle(&si1, hWrite, STD_OUTPUT_HANDLE);

	bRes = CreateProcess(
		NULL,
		command1,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&si1,
		&pi1);
	DIE(bRes == FALSE, "Create Process1");

	CloseHandle(hWrite);

	/**
	 * TODO 2 - Launch command2 with stdin redirected out of the pipe
	 */
	RedirectHandle(&si2, hRead, STD_INPUT_HANDLE);

	bRes = CreateProcess(
		NULL,
		command2,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&si2,
		&pi2);
	DIE(bRes == FALSE, "Create Process2");

	CloseHandle(hRead);

	/**
	 * TODO 2 - Wait for processes 
	 */
	dwRes = WaitForSingleObject(pi1.hProcess, INFINITE);
	DIE(dwRes == WAIT_FAILED, "WaitForSingleObject");

	CloseProcess(&pi1);

	dwRes = WaitForSingleObject(pi2.hProcess, INFINITE);
	DIE(dwRes == WAIT_FAILED, "WaitForSingleObject");

	CloseProcess(&pi2);
}

int main(int argc, char **argv)
{
	CHAR line[MAX_LINE_SIZE];

	AllocateMemory();

	while (1) {
		printf("> ");
		fflush(stdout);

		ZeroMemory(line, MAX_LINE_SIZE);

		if (fgets(line, sizeof(line), stdin) == NULL)
			   exit(EXIT_SUCCESS);

		type = ParseLine(line);

		switch (type) {
		case EXIT_CMD:
			exit(EXIT_SUCCESS);

		case SET_VAR:
			SetEnvironmentVariable(var, value);
			break;

		case PIPE:
			PipeCommands(command1, command2);
			break;
	
		case SIMPLE:
			RunSimpleCommand(command1);
			break;
		}
	}

	Cleanup();
	return 0;
}

static VOID AllocateMemory(VOID)
{
	command1 = malloc(256 * sizeof(char));
	command2 = malloc(256 * sizeof(char));
}

VOID Cleanup(VOID)
{
	free(command1);
	free(command2);
}

static HANDLE MyOpenFile(PCSTR filename)
{
	SECURITY_ATTRIBUTES sa;

	ZeroMemory(&sa, sizeof(sa));
	sa.bInheritHandle = TRUE;

	return CreateFile(
		filename,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		&sa,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}

static INT ParseLine(PCHAR line)
{
	
	INT ret = SIMPLE;
	PCHAR pos;

	stdinFilename = NULL;
	stdoutFilename = NULL;
	stderrFilename = NULL;

	ZeroMemory(command1, MAX_CMD_SIZE);
	ZeroMemory(command2, MAX_CMD_SIZE);

	/* Empty command */
	if (strcmp(line, "\n") == 0)
		return ERROR;

	/* Exit command */
	if (strncmp("exit", line, strlen("exit")) == 0)
		return EXIT_CMD;

	/* var = value */
	if (strchr(line, '=') != 0) {
		PCHAR delim = "=\n";
		PCHAR nextToken;
		PCHAR token = strtok_s(line, delim, &nextToken);

		if (token == NULL)
			return 0;

		var = _strdup(token);

		token = strtok_s(NULL, delim, &nextToken);
		if (token == NULL)
			return 0;

		value = _strdup(token);
		return SET_VAR;
	}

	/* Verify | */
	pos = strchr(line, '|');
	if (pos != 0) {
		strncat_s(command1, MAX_CMD_SIZE, line,
				strlen(line) - strlen(pos));
	
		/* Skip | */
		pos++;
		ParseSimpleCommand(pos, &command2);

		return PIPE;
	}

	ParseSimpleCommand(line, &command1);

	return SIMPLE;
}

static INT ParseSimpleCommand(PCHAR line, PCHAR *command)
{
	PCSTR delim = " \t\n";
	PCHAR nextToken;
	PCHAR token = strtok_s(line, delim, &nextToken);

	if (token == NULL)
		return ERROR;

	/* Go through args + expand them + check for redirection */
	while (token != NULL) {
		if (token[0] == '$') {
			token = ExpandVariable(token+1);

			if (token == NULL) {
				printf(" Expansion failed\n");
				return ERROR;
			}
		}

		if (strncmp(token, ">", strlen(">")) == 0) {
			if (strlen(token) > strlen(">")) {
				token++;
				stdoutFilename = _strdup(token);
			} else {
				token = strtok_s(NULL, delim, &nextToken);
				stdoutFilename = _strdup(token);
			}
		} else {
			strcat_s(*command, MAX_CMD_SIZE, token);
			strcat_s(*command, MAX_CMD_SIZE, " ");
		}

		token = strtok_s(NULL, delim, &nextToken);
	}

	return SIMPLE;
}
