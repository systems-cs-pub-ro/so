/**
 * Filename: debug.h
 * Author: Alexandru Mosoi (brtzsnr@gmail.com)
 * Last modified: 07 March 2008
 *
 * Documentation:
 *
 * This module offers students some simple C macros meant to facilitate
 * treating errors and debugging. Messages will be printed to stderr and
 * will contain, alongside user's message, file, function and line where
 * the macro was called.
 * 
 * LOG(level, format, params)
 * - Print message of level 'level' if 'level' is equal or less than 'LOG_LEVEL'
 * - There are 4 default levels: FATAL, WARN, INFO, DEBUG
 * - 'LOG_LEVEL' is a macro. If not defined when including 'log.h' it will take
 * 		default value 'WARN'
 * - 'format' and 'params' have the same meaning as functions similar to 'printf()'.
 * 		Internally, params sorted by 'level' are sent to 'fprintf()'.
 * - After printing messages from 'FATAL' level, program exits.
 * 
 * LOG_ERROR(format, params)
 * - Similar to 'LOG(FATAL, fomat, params)', but also prints last error.
 * 
 * DLOG(level, format, params)
 * - Prints a debug message of level 'DEBUG + level'
 * 
 * ASSERT(cond)
 * - If 'cond' fails, print an error message (with cond and last
 * 		system error), then exits the program.
 * 
 * CHECK_op(left, right)
 * - Checks that binary operation 'op' is successful
 * - 'op' can be:
 * 		- EQ for 'EQual'
 * 		- NE for 'Not Equal'
 * 
 * To modify the value of LOG_LEVEL add the following option when compiling:
 * -D LOG_LEVEL=value (when using GCC/Linux)
 * /D LOG_LEVEL=value (when using MSVS/Windows)
 * 
 * Examples:
 *
 * // LOG_LEVEL = FATAL
 * LOG(INFO, "result = %d", 3);    // prints nothing
 * LOG(DEBUG, "result = %d", 3);   // prints nothing
 * DLOG(666, "result = %d", 3);    // prints nothing
 * ASSERT(3 == 4);                 // error, exits program
 *
 * // LOG_LEVEL = DEBUG
 * LOG(INFO, "result = %d", 3);    // prints 'result = 3'
 * LOG(DEBUG, "result = %d", 3);   // prints 'result = 3'
 * DLOG(666, "result = %d", 3);    // prints nothing
 * ASSERT(3 == 4);                 // error, exits program
 *
 * // LOG_LEVEL = (DEBUG+1024)
 * LOG(INFO, "result = %d", 3);    // prints 'result = 3'
 * LOG(DEBUG, "result = %d", 3);   // prints 'result = 3'
 * DLOG(666, "result = %d", 3);    // prints 'result = 3'
 * ASSERT(3 == 4);                 // error, exits program
 *
 * CHECK_NE(fd = open("file", O_RDONLY), -1);
 * CHECK_EQ(function(), 0)
 *
 * // Following lines exit program
 * // if 'f()' returns 0.
 *
 * if (f() == 0)
 * 		LOG(FATAL, "f() == 0");
 * CHECK_NE(f(), 0);
 * ASSERT(f() != 0);
 *
 * Examples of messages:
 *
 * LOG(WARN, "Warning message");
 * //  W main.c (main) 9: Warning message
 *
 * ASSERT(0 == 1);
 * //  F main.c (main) 11: ASSERT failed: 0 == 1 (last error = Success)
 *
 * CHECK_NE(open("does_not_exist", O_RDONLY), -1);
 * //  F main.c (main) 15: CHECK failed: open("does_not_exist", 00) != -1 (last error = No such file or directory)
 *
 * DLOG(99, "All work and no play makes %s a dull %s!", "Jack", "boy");
 * // 99 main.c (main) 15: All work and no play makes Jack a dull boy!
 *
 * open("does_not_exist", O_RDONLY);
 * LOG_ERROR("%s", "Error message");
 * //    main.c (main) 18: Error message
 * //  F main.c (main) 18: Last error: No such file or directory.
 */
#ifndef LOG_H__
#define LOG_H__

#if !defined(__GNUC__) && !defined(_WIN32)
#error Unknown operating system. Please check this file.
#endif

#include <stdio.h>
#include <stdlib.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL (WARN + 1024)
#endif

#ifdef __GNUC__  /* Linux */

#include <errno.h>
#include <string.h>

#define __FUNCTION__ __func__
#define LOG_GET_ERROR_MESSAGE							\
	char *error_buffer;							\
	char error_buffer2[512];						\
	error_buffer = error_buffer2;						\
	error_buffer = strerror_r(errno, error_buffer, sizeof(error_buffer));

#else  /* Windows */

#include <windows.h>

#define LOG_GET_ERROR_MESSAGE
	char error_buffer[512];							\
	FormatMessage(								\
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,	\
		NULL,								\
		GetLastError(),							\
		0,								\
		error_buffer,							\
		sizeof(error_buffer),						\
		NULL);
		NULL);

#endif

enum LogType {
	NONE,
	FATAL,
	WARN,
	INFO,
	DEBUG
};

#define LOG(type, ...)							\
	do {								\
		if (type <= LOG_LEVEL) {				\
			if (type <= DEBUG)				\
				fprintf(stderr, " %c", " FWID"[type]);	\
			else						\
				fprintf(stderr, "%02d", type - DEBUG);	\
									\
			fprintf(stderr, " %s (%s) %d: ", __FILE__,	\
				__FUNCTION__, __LINE__);		\
			fprintf(stderr, __VA_ARGS__);			\
			fprintf(stderr, "\n");				\
			fflush(stderr);					\
									\
			/* Abort() is C89 compliant. */			\
			if (type == FATAL)				\
				abort();				\
		}							\
	} while (0)

#define LOG_ERROR(...)						\
	do {							\
		LOG_GET_ERROR_MESSAGE				\
		LOG(NONE, __VA_ARGS__);				\
		LOG(FATAL, "Last error: %s.", error_buffer);	\
	} while (0)

#define DLOG(level, ...) LOG(DEBUG + (level), __VA_ARGS__)

#define ASSERT(condition)						\
	do {								\
		if (!(condition)) {					\
			LOG_GET_ERROR_MESSAGE				\
			LOG(NONE, "ASSERT failed: %s.", #condition);	\
			LOG(FATAL, "Last error: %s.", error_buffer);	\
		}							\
	} while (0)

#define CHECK(left, operation, right)					\
	do {								\
		if (!((left)operation(right))) {			\
			LOG_GET_ERROR_MESSAGE				\
			LOG(NONE, "CHECK failed: %s %s %s.", #left,	\
				#operation, #right);			\
			LOG(FATAL, "Last error: %s.", error_buffer);	\
		}							\
	} while (0)

#define CHECK_EQ(left, right) CHECK(left, ==, right)
#define CHECK_NE(left, right) CHECK(left, !=, right)

#endif
