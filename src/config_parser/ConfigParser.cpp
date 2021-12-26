/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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
     CGI_INTERPRETER = 279,
     CGI_TIMEOUT = 280,
     STATE = 281,
     NUMBER = 282,
     WORD = 283,
     PATH = 284,
     URL = 285,
     FILESIZE = 286,
     HTTP_METHOD = 287,
     IP = 288,
     IDLE_TIMEOUT = 289,
     KA_TIMEOUT = 290,
     KA_TIME = 291,
     KA_PROBES = 292,
     KA_INTERVAL = 293,
     URI_LENGTH = 294,
     REQUEST_LENGTH = 295,
     EQUALS = 296,
     TILDE = 297
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
#define CGI_INTERPRETER 279
#define CGI_TIMEOUT 280
#define STATE 281
#define NUMBER 282
#define WORD 283
#define PATH 284
#define URL 285
#define FILESIZE 286
#define HTTP_METHOD 287
#define IP 288
#define IDLE_TIMEOUT 289
#define KA_TIMEOUT 290
#define KA_TIME 291
#define KA_PROBES 292
#define KA_INTERVAL 293
#define URI_LENGTH 294
#define REQUEST_LENGTH 295
#define EQUALS 296
#define TILDE 297




/* Copy the first part of user declarations.  */
#line 1 "src/config_parser/YACC.y"

#include <stdio.h>
#include <stdlib.h>
#include "../../inc/Webserv.hpp"
extern char	*yytext;


extern "C"
{
	void yyerror(Config *config, const char *str)
	{
		(void)config;
		(void)str;
	}
	int yyparse(void);
	int yylex(void); 
    int yywrap()
    {
		return 1;
    }
}

#ifdef DEBUG_LEX
extern int yydebug;

int main(void)
{
        yydebug=1;
        yyparse();
}
#endif



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 46 "src/config_parser/YACC.y"
{
    int				number;
    char *			string;
	char			letter;
}
/* Line 193 of yacc.c.  */
#line 220 "src/config_parser/ConfigParser.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 233 "src/config_parser/ConfigParser.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   82

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNRULES -- Number of states.  */
#define YYNSTATES  113

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   297

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     4,     8,    10,    12,    14,    16,    18,
      21,    23,    26,    28,    30,    32,    34,    36,    38,    40,
      45,    46,    50,    51,    55,    56,    59,    60,    66,    67,
      71,    73,    75,    77,    79,    81,    83,    85,    87,    90,
      91,    94,    97,   101,   103,   106,   109,   112,   115,   119,
     120,   123,   124,   132,   133,   141,   143,   145,   146,   147,
     151,   153,   155,   157,   159,   161,   163,   165,   167,   170,
     173,   176,   177,   180,   181,   185,   187,   190,   195,   199,
     202,   205
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      44,     0,    -1,    -1,    44,    45,    22,    -1,    46,    -1,
      47,    -1,    48,    -1,    50,    -1,    55,    -1,     3,    29,
      -1,     5,    -1,    49,    27,    -1,    34,    -1,    35,    -1,
      36,    -1,    37,    -1,    38,    -1,    39,    -1,    40,    -1,
       6,    20,    51,    21,    -1,    -1,    51,    52,    22,    -1,
      -1,    29,    53,    54,    -1,    -1,    54,    28,    -1,    -1,
      12,    20,    56,    57,    21,    -1,    -1,    57,    58,    22,
      -1,    59,    -1,    61,    -1,    63,    -1,    64,    -1,    65,
      -1,    66,    -1,    67,    -1,    69,    -1,    14,    60,    -1,
      -1,    60,    29,    -1,    13,    62,    -1,    33,    23,    27,
      -1,    27,    -1,     4,    29,    -1,    17,    31,    -1,     7,
      29,    -1,    11,    26,    -1,    16,    68,    29,    -1,    -1,
      68,    27,    -1,    -1,    15,    72,    29,    20,    70,    73,
      21,    -1,    -1,    15,    42,    28,    20,    71,    73,    21,
      -1,    41,    -1,    42,    -1,    -1,    -1,    73,    74,    22,
      -1,    75,    -1,    76,    -1,    79,    -1,    82,    -1,    77,
      -1,    64,    -1,    83,    -1,    84,    -1,    11,    26,    -1,
       7,    29,    -1,     9,    78,    -1,    -1,    78,    29,    -1,
      -1,     8,    80,    81,    -1,    32,    -1,    81,    32,    -1,
      10,    27,    30,    18,    -1,    10,    27,    30,    -1,    24,
      29,    -1,    24,    28,    -1,    25,    27,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    77,    77,    78,    81,    83,    85,    87,    89,    93,
     101,   107,   138,   139,   140,   141,   142,   143,   144,   148,
     150,   151,   155,   154,   161,   162,   170,   169,   175,   176,
     179,   181,   183,   185,   187,   189,   191,   193,   197,   199,
     200,   208,   211,   218,   225,   233,   255,   262,   269,   278,
     279,   287,   286,   308,   307,   318,   319,   320,   322,   323,
     326,   328,   330,   332,   334,   336,   338,   340,   343,   349,
     356,   358,   359,   367,   366,   373,   378,   385,   392,   400,
     406,   413
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ERRLOG", "ACCLOG", "DAEMON",
  "MIMETYPES", "ROOT", "ALLOWED_METHODS", "INDEX", "REDIRECTION",
  "AUTOINDEX", "SERVER", "LISTEN", "SERVER_NAME", "LOCATION", "ERROR_PAGE",
  "CLIENT_MAX_BODY_SIZE", "URI", "QUOTE", "OBRACE", "EBRACE", "SEMICOLON",
  "COLON", "CGI_INTERPRETER", "CGI_TIMEOUT", "STATE", "NUMBER", "WORD",
  "PATH", "URL", "FILESIZE", "HTTP_METHOD", "IP", "IDLE_TIMEOUT",
  "KA_TIMEOUT", "KA_TIME", "KA_PROBES", "KA_INTERVAL", "URI_LENGTH",
  "REQUEST_LENGTH", "EQUALS", "TILDE", "$accept", "commands", "command",
  "error_log_file", "daemon", "num_constants", "num_constant",
  "mime_types", "mime_type_statements", "mime_type_statement", "@1",
  "extensions", "server", "@2", "server_statements", "server_statement",
  "server_names", "server_name", "listen", "what_to_listen",
  "access_log_file", "client_max_body_size", "root", "autoindex",
  "error_page", "error_num", "location_block", "@3", "@4", "loc_type",
  "location_statements", "location_statement", "autoindex_loc", "root_loc",
  "index_loc", "index_file", "allowed_methods", "@5", "http_methods",
  "http_redirection", "cgi_interpreter", "cgi_timeout", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    43,    44,    44,    45,    45,    45,    45,    45,    46,
      47,    48,    49,    49,    49,    49,    49,    49,    49,    50,
      51,    51,    53,    52,    54,    54,    56,    55,    57,    57,
      58,    58,    58,    58,    58,    58,    58,    58,    59,    60,
      60,    61,    62,    62,    63,    64,    65,    66,    67,    68,
      68,    70,    69,    71,    69,    72,    72,    72,    73,    73,
      74,    74,    74,    74,    74,    74,    74,    74,    75,    76,
      77,    78,    78,    80,    79,    81,    81,    82,    82,    83,
      83,    84
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     3,     1,     1,     1,     1,     1,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     4,
       0,     3,     0,     3,     0,     2,     0,     5,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     0,
       2,     2,     3,     1,     2,     2,     2,     2,     3,     0,
       2,     0,     7,     0,     7,     1,     1,     0,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     0,     2,     0,     3,     1,     2,     4,     3,     2,
       2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,    10,     0,     0,    12,    13,    14,
      15,    16,    17,    18,     0,     4,     5,     6,     0,     7,
       8,     9,    20,    26,     3,    11,     0,    28,    19,    22,
       0,     0,    24,    21,     0,     0,     0,     0,    39,    57,
      49,     0,    27,     0,    30,    31,    32,    33,    34,    35,
      36,    37,    23,    44,    46,    47,    43,     0,    41,    38,
      55,    56,     0,     0,    45,    29,    25,     0,    40,     0,
       0,    50,    48,    42,    53,    51,    58,    58,     0,     0,
       0,    73,    71,     0,     0,    54,     0,     0,    65,     0,
      60,    61,    64,    62,    63,    66,    67,    52,    69,     0,
      70,     0,    68,    80,    79,    81,    59,    75,    74,    72,
      78,    76,    77
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,    14,    15,    16,    17,    18,    19,    26,    30,
      32,    52,    20,    27,    31,    43,    44,    59,    45,    58,
      46,    88,    48,    49,    50,    63,    51,    77,    76,    62,
      78,    89,    90,    91,    92,   100,    93,    99,   108,    94,
      95,    96
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -32
static const yytype_int8 yypact[] =
{
     -32,     0,   -32,   -22,   -32,   -16,    -1,   -32,   -32,   -32,
     -32,   -32,   -32,   -32,     2,   -32,   -32,   -32,    -2,   -32,
     -32,   -32,   -32,   -32,   -32,   -32,   -20,   -32,   -32,   -32,
       4,    49,   -32,   -32,     3,    17,     7,   -25,   -32,   -31,
     -32,    -3,   -32,    25,   -32,   -32,   -32,   -32,   -32,   -32,
     -32,   -32,     1,   -32,   -32,   -32,   -32,    26,   -32,    19,
     -32,    22,    23,    -9,   -32,   -32,   -32,    27,   -32,    37,
      41,   -32,   -32,   -32,   -32,   -32,   -32,   -32,     6,    34,
      38,   -32,   -32,    42,    45,   -32,    -7,    46,   -32,    50,
     -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,    36,
      47,    44,   -32,   -32,   -32,   -32,   -32,   -32,    43,   -32,
      59,   -32,   -32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,
     -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,
     -32,    48,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,
       5,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,
     -32,   -32
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       2,    28,    56,     3,    22,     4,     5,    21,    57,    29,
      60,    61,     6,    80,    81,    82,    83,    84,    71,    23,
      72,   103,   104,    41,    24,    25,    33,    85,    64,    66,
      86,    87,    53,    55,     7,     8,     9,    10,    11,    12,
      13,    80,    81,    82,    83,    84,    54,    65,    68,    67,
      69,    41,    70,    34,    73,    97,    35,    74,    86,    87,
      36,    75,    37,    38,    39,    40,    41,    98,   107,   101,
      42,   102,   106,   105,   110,   111,   109,   112,     0,    47,
       0,     0,    79
};

static const yytype_int8 yycheck[] =
{
       0,    21,    27,     3,    20,     5,     6,    29,    33,    29,
      41,    42,    12,     7,     8,     9,    10,    11,    27,    20,
      29,    28,    29,    17,    22,    27,    22,    21,    31,    28,
      24,    25,    29,    26,    34,    35,    36,    37,    38,    39,
      40,     7,     8,     9,    10,    11,    29,    22,    29,    23,
      28,    17,    29,     4,    27,    21,     7,    20,    24,    25,
      11,    20,    13,    14,    15,    16,    17,    29,    32,    27,
      21,    26,    22,    27,    30,    32,    29,    18,    -1,    31,
      -1,    -1,    77
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    44,     0,     3,     5,     6,    12,    34,    35,    36,
      37,    38,    39,    40,    45,    46,    47,    48,    49,    50,
      55,    29,    20,    20,    22,    27,    51,    56,    21,    29,
      52,    57,    53,    22,     4,     7,    11,    13,    14,    15,
      16,    17,    21,    58,    59,    61,    63,    64,    65,    66,
      67,    69,    54,    29,    29,    26,    27,    33,    62,    60,
      41,    42,    72,    68,    31,    22,    28,    23,    29,    28,
      29,    27,    29,    27,    20,    20,    71,    70,    73,    73,
       7,     8,     9,    10,    11,    21,    24,    25,    64,    74,
      75,    76,    77,    79,    82,    83,    84,    21,    29,    80,
      78,    27,    26,    28,    29,    27,    22,    32,    81,    29,
      30,    32,    18
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (config, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, config); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, Config *config)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, config)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    Config *config;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (config);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, Config *config)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, config)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    Config *config;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, config);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, Config *config)
#else
static void
yy_reduce_print (yyvsp, yyrule, config)
    YYSTYPE *yyvsp;
    int yyrule;
    Config *config;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , config);
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, config); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, Config *config)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, config)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    Config *config;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (config);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (Config *config);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (Config *config)
#else
int
yyparse (config)
    Config *config;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 9:
#line 94 "src/config_parser/YACC.y"
    {
			config->err_log = (yyvsp[(2) - (2)].string);
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 10:
#line 102 "src/config_parser/YACC.y"
    {
			config->daemon = true;
		}
    break;

  case 11:
#line 108 "src/config_parser/YACC.y"
    {
			switch ((yyvsp[(1) - (2)].number))
			{
			case IDLE_TIMEOUT:
				config->timeout_idle = (yyvsp[(2) - (2)].number);
				break;
			case KA_TIMEOUT:
				config->timeout_ka = (yyvsp[(2) - (2)].number);
				break;
			case KA_TIME:
				config->keepalive_time = (yyvsp[(2) - (2)].number);
				break;
			case KA_PROBES:
				config->num_probes = (yyvsp[(2) - (2)].number);
				break;
			case KA_INTERVAL:
				config->keepalive_intvl = (yyvsp[(2) - (2)].number);
				break;
			case URI_LENGTH:
				config->limit_uri_length = (yyvsp[(2) - (2)].number);
				break;
			case REQUEST_LENGTH:
				config->limit_headers_length = (yyvsp[(2) - (2)].number);
				if (config->incoming_buffer < (yyvsp[(2) - (2)].number))
					config->incoming_buffer = (yyvsp[(2) - (2)].number);
				break;
			}
		}
    break;

  case 12:
#line 138 "src/config_parser/YACC.y"
    { (yyval.number) = IDLE_TIMEOUT; }
    break;

  case 13:
#line 139 "src/config_parser/YACC.y"
    { (yyval.number) = KA_TIMEOUT; }
    break;

  case 14:
#line 140 "src/config_parser/YACC.y"
    { (yyval.number) = KA_TIME; }
    break;

  case 15:
#line 141 "src/config_parser/YACC.y"
    { (yyval.number) = KA_PROBES; }
    break;

  case 16:
#line 142 "src/config_parser/YACC.y"
    { (yyval.number) = KA_INTERVAL; }
    break;

  case 17:
#line 143 "src/config_parser/YACC.y"
    { (yyval.number) = URI_LENGTH; }
    break;

  case 18:
#line 144 "src/config_parser/YACC.y"
    { (yyval.number) = REQUEST_LENGTH; }
    break;

  case 22:
#line 155 "src/config_parser/YACC.y"
    {
			config->mime_temp = std::string((yyvsp[(1) - (1)].string));
			free((yyvsp[(1) - (1)].string));
		}
    break;

  case 25:
#line 163 "src/config_parser/YACC.y"
    {
			config->mime_types.insert(std::pair<std::string, std::string>((yyvsp[(2) - (2)].string), config->mime_temp));
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 26:
#line 170 "src/config_parser/YACC.y"
    {
			config->servers.push_back(ConfigServ());		
		}
    break;

  case 40:
#line 201 "src/config_parser/YACC.y"
    {
			config->servers.back().server_names.push_back((yyvsp[(2) - (2)].string));
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 42:
#line 212 "src/config_parser/YACC.y"
    {
			config->servers.back().ip = (yyvsp[(1) - (3)].string);
			free((yyvsp[(1) - (3)].string));
			config->servers.back().port = (yyvsp[(3) - (3)].number);
		}
    break;

  case 43:
#line 219 "src/config_parser/YACC.y"
    {
			config->servers.back().port = (yyvsp[(1) - (1)].number);
		}
    break;

  case 44:
#line 226 "src/config_parser/YACC.y"
    {
			config->servers.back().acc_log = (yyvsp[(2) - (2)].string);
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 45:
#line 234 "src/config_parser/YACC.y"
    {
			int l = strlen((yyvsp[(2) - (2)].string));
			int factor = 1;
			switch ((yyvsp[(2) - (2)].string)[l - 1])
			{
			case 'G':
				factor *= 1024;
			case 'M':
				factor *= 1024;
			case 'K':
				factor *= 1024;
			}
			(yyvsp[(2) - (2)].string)[l - 1] = 0;
			if (config->servers.back().locations.size() == 0)
				config->servers.back().client_max_body_size = factor * atoi((yyvsp[(2) - (2)].string));
			else
				config->servers.back().locations.back().setMaxBodySize(factor * atoi((yyvsp[(2) - (2)].string)));
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 46:
#line 256 "src/config_parser/YACC.y"
    {
			config->servers.back().root = (yyvsp[(2) - (2)].string);
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 47:
#line 263 "src/config_parser/YACC.y"
    {
			config->servers.back().autoindex = (yyvsp[(2) - (2)].number);
		}
    break;

  case 48:
#line 270 "src/config_parser/YACC.y"
    {
			std::vector<int> * ptr_vec = &config->servers.back().err_num_temp;
			std::map<int, std::string> * ptr_map = &config->servers.back().err_pages;
			for (std::vector<int>::iterator it = (*ptr_vec).begin(); it != (*ptr_vec).end(); ++it)
				(*ptr_map).insert(std::pair<int, std::string>(*it, (yyvsp[(3) - (3)].string)));
			free((yyvsp[(3) - (3)].string));
		}
    break;

  case 50:
#line 280 "src/config_parser/YACC.y"
    {
			config->servers.back().err_num_temp.push_back((yyvsp[(2) - (2)].number));
		}
    break;

  case 51:
#line 287 "src/config_parser/YACC.y"
    {
			char type = (yyvsp[(2) - (4)].letter);
			std::vector<std::string> loc_path;
			if (type == location::pathType::extention)
			{
				char * token = strtok((yyvsp[(3) - (4)].string), "/");
				while (token != NULL) {
					loc_path.push_back(token);
					token = strtok(NULL, " ");
				}
			}
			else
				loc_path.push_back((yyvsp[(3) - (4)].string));
			config->servers.back().locations.push_back(Location(type, loc_path, \
							config->servers.back().root, config->servers.back().autoindex, \
							config->servers.back().client_max_body_size));
			free((yyvsp[(3) - (4)].string));
		}
    break;

  case 53:
#line 308 "src/config_parser/YACC.y"
    {
			std::vector<std::string> loc_path;
			loc_path.push_back((yyvsp[(3) - (4)].string));
			config->servers.back().locations.push_back(Location(location::pathType::extention, loc_path, \
							config->servers.back().root, config->servers.back().autoindex, \
							config->servers.back().client_max_body_size));
			free((yyvsp[(3) - (4)].string));
		}
    break;

  case 55:
#line 318 "src/config_parser/YACC.y"
    { (yyval.letter) = location::pathType::equal; }
    break;

  case 56:
#line 319 "src/config_parser/YACC.y"
    { (yyval.letter) = location::pathType::extention; }
    break;

  case 57:
#line 320 "src/config_parser/YACC.y"
    { (yyval.letter) = location::pathType::partial; }
    break;

  case 68:
#line 344 "src/config_parser/YACC.y"
    {
			config->servers.back().locations.back().setAutoindex((yyvsp[(2) - (2)].number));
		}
    break;

  case 69:
#line 350 "src/config_parser/YACC.y"
    {
			config->servers.back().locations.back().setRoot((yyvsp[(2) - (2)].string));
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 72:
#line 360 "src/config_parser/YACC.y"
    {
			config->servers.back().locations.back().addIndex((yyvsp[(2) - (2)].string));
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 73:
#line 367 "src/config_parser/YACC.y"
    {
			config->servers.back().locations.back().clearMethodSet();
		}
    break;

  case 75:
#line 374 "src/config_parser/YACC.y"
    {
			config->servers.back().locations.back().addMethod((yyvsp[(1) - (1)].string));
			free((yyvsp[(1) - (1)].string));
		}
    break;

  case 76:
#line 379 "src/config_parser/YACC.y"
    {
			config->servers.back().locations.back().addMethod((yyvsp[(2) - (2)].string));
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 77:
#line 386 "src/config_parser/YACC.y"
    {
			Redirect * ptr = new Redirect((yyvsp[(2) - (4)].number), (yyvsp[(3) - (4)].string), true);
			config->servers.back().locations.back().setRedir(ptr);
			free((yyvsp[(3) - (4)].string));
		}
    break;

  case 78:
#line 393 "src/config_parser/YACC.y"
    {
			Redirect * ptr = new Redirect((yyvsp[(2) - (3)].number), (yyvsp[(3) - (3)].string), false);
			config->servers.back().locations.back().setRedir(ptr);
			free((yyvsp[(3) - (3)].string));
		}
    break;

  case 79:
#line 401 "src/config_parser/YACC.y"
    {
			config->servers.back().locations.back().setCgiInterpreter((yyvsp[(2) - (2)].string));
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 80:
#line 407 "src/config_parser/YACC.y"
    {
			config->servers.back().locations.back().setCgiInterpreter((yyvsp[(2) - (2)].string));
			free((yyvsp[(2) - (2)].string));
		}
    break;

  case 81:
#line 414 "src/config_parser/YACC.y"
    {
			config->servers.back().locations.back().setCgiTimeout((yyvsp[(2) - (2)].number));
		}
    break;


/* Line 1267 of yacc.c.  */
#line 1879 "src/config_parser/ConfigParser.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (config, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (config, yymsg);
	  }
	else
	  {
	    yyerror (config, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, config);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, config);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (config, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, config);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, config);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 418 "src/config_parser/YACC.y"


int yylex(void);  

