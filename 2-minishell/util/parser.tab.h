
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
	enum yytokentype {
		NOT_ACCEPTED_CHAR = 258,
		INVALID_ENVIRONMENT_VAR = 259,
		UNEXPECTED_EOF = 260,
		CHARS_AFTER_EOL = 261,
		END_OF_FILE = 262,
		END_OF_LINE = 263,
		BLANK = 264,
		REDIRECT_OE = 265,
		REDIRECT_O = 266,
		REDIRECT_E = 267,
		INDIRECT = 268,
		REDIRECT_APPEND_E = 269,
		REDIRECT_APPEND_O = 270,
		WORD = 271,
		ENV_VAR = 272,
		SEQUENTIAL = 273,
		PARALLEL = 274,
		CONDITIONAL_ZERO = 275,
		CONDITIONAL_NZERO = 276,
		PIPE = 277
};
#endif



#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE {

/* Line 1676 of yacc.c  */
#line 221 "parser.y"

	command_t *command_un;
	const char *string_un;
	redirect_t redirect_un;
	simple_command_t *simple_command_un;
	word_t *exe_un;
	word_t *params_un;
	word_t *word_un;



/* Line 1676 of yacc.c  */
#line 86 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#if !defined YYLTYPE && !defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE {
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;

