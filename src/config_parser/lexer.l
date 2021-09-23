%{
	#define YYSTYPE char *
	#include <stdio.h>
	//DELETE THIS SHIT ON MAC	
	#include <vector>
	#include "../inc/Config.hpp"
	//IM SERIOUS
	#include "YACC.hh"
	int yylex(void);  
%}

%%
	/*
	server | 
	listen | 
	server_name | 
	location | 
	error_page | 
	client_max_body_size 			printf(" DIRECTIVE");
	*/
GET | 
POST |
DELETE					{ yylval=strdup(yytext); return HTTP_METHOD; }
root					{/*printf("ROOT");*/return ROOT;}
autoindex				return AUTOINDEX;
limit_except			return LIMIT_EXCEPT;
on|off					{ yylval=strdup(yytext); return STATE; }	
server					return SERVER;
listen					return LISTEN;
server_name				return SERVER_NAME;
location				return LOCATION;
error_page				return ERROR_PAGE;
client_max_body_size	return CLIENT_MAX_BODY_SIZE;
[0-9]+[.][0-9]+[.][0-9]+[.][0-9]+	 { yylval=strdup(yytext); return IP; }
=						return EQUAL;
:						return COLON;
[0-9]+  				{ yylval=strdup(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z0-9_]*   { yylval=strdup(yytext); return WORD; }
[a-zA-Z0-9\/.-]+        { yylval=strdup(yytext); return PATH; }
"#".*					;
\"                      return QUOTE;
\{                      return OBRACE;
\}                      return EBRACE;
;                       return SEMICOLON;
\n                      ;/* пропускаем концы строк */;
[ \t]+                  /* пропускаем пробелы */;
%%

#define YYSTYPE char *
