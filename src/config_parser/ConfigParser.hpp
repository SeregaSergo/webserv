/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     ERRLOG = 258,
     ACCLOG = 259,
     DAEMON = 260,
     MIMETYPES = 261,
     ROOT = 262,
     ALLOWED_METHODS = 263,
     INDEX = 264,
     REDIRECTION = 265,
     AUTOINDEX = 266,
     SERVER = 267,
     LISTEN = 268,
     SERVER_NAME = 269,
     LOCATION = 270,
     ERROR_PAGE = 271,
     CLIENT_MAX_BODY_SIZE = 272,
     URI = 273,
     QUOTE = 274,
     OBRACE = 275,
     EBRACE = 276,
     SEMICOLON = 277,
     COLON = 278,
     CGI_ENABLED = 279,
     CGI_TIMEOUT = 280,
     SESSIONS_ON = 281,
     STATE = 282,
     NUMBER = 283,
     WORD = 284,
     PATH = 285,
     URL = 286,
     FILESIZE = 287,
     HTTP_METHOD = 288,
     IP = 289,
     IDLE_TIMEOUT = 290,
     KA_TIMEOUT = 291,
     SESSION_TIMEOUT = 292,
     KA_TIME = 293,
     KA_PROBES = 294,
     KA_INTERVAL = 295,
     URI_LENGTH = 296,
     REQUEST_LENGTH = 297,
     EQUALS = 298,
     TILDE = 299
   };
#endif
/* Tokens.  */
#define ERRLOG 258
#define ACCLOG 259
#define DAEMON 260
#define MIMETYPES 261
#define ROOT 262
#define ALLOWED_METHODS 263
#define INDEX 264
#define REDIRECTION 265
#define AUTOINDEX 266
#define SERVER 267
#define LISTEN 268
#define SERVER_NAME 269
#define LOCATION 270
#define ERROR_PAGE 271
#define CLIENT_MAX_BODY_SIZE 272
#define URI 273
#define QUOTE 274
#define OBRACE 275
#define EBRACE 276
#define SEMICOLON 277
#define COLON 278
#define CGI_ENABLED 279
#define CGI_TIMEOUT 280
#define SESSIONS_ON 281
#define STATE 282
#define NUMBER 283
#define WORD 284
#define PATH 285
#define URL 286
#define FILESIZE 287
#define HTTP_METHOD 288
#define IP 289
#define IDLE_TIMEOUT 290
#define KA_TIMEOUT 291
#define SESSION_TIMEOUT 292
#define KA_TIME 293
#define KA_PROBES 294
#define KA_INTERVAL 295
#define URI_LENGTH 296
#define REQUEST_LENGTH 297
#define EQUALS 298
#define TILDE 299




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 46 "src/config_parser/YACC.y"
{
    int				number;
    char *			string;
	char			letter;
}
/* Line 1529 of yacc.c.  */
#line 143 "src/config_parser/ConfigParser.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

