%{
	#include <stdio.h>
	//DELETE THIS SHIT ON MAC
	//IM SERIOUS
	#include "ConfigParser.hpp"
	int yylex(void);  
%}

%option nounput
%option noinput

%%
	/*
	server | 
	listen | 
	server_name | 
	location | 
	error_page | 
	client_max_body_size 			printf(" DIRECTIVE");
	*/

error_log					return ERRLOG;
access_log					return ACCLOG;
daemon_mode					return DAEMON;
idle_timeout				return IDLE_TIMEOUT;
keepalive_timeout			return KA_TIMEOUT;
tcp_keepalive_time			return KA_TIME;
tcp_keepalive_probes		return KA_PROBES;
tcp_keepalive_intvl			return KA_INTERVAL;
types						return MIMETYPES;
root						return ROOT;
allowed_methods				return ALLOWED_METHODS;
index						return INDEX;
redirect					return REDIRECTION; 
autoindex					return AUTOINDEX;
server						return SERVER;
listen						return LISTEN;
server_name					return SERVER_NAME;
location					return LOCATION;
error_page					return ERROR_PAGE;
client_max_body_size		return CLIENT_MAX_BODY_SIZE;
$uri						return URI;
\"                      	return QUOTE;
\{                      	return OBRACE;
\}                      	return EBRACE;
;                       	return SEMICOLON;
on|off						{ yylval.number=!strcmp(yytext,"on"); return STATE; }
GET|POST|DELETE				{ yylval.string=strdup(yytext); return HTTP_METHOD; }	
[0-9]+(B|K|M|G)				{ yylval.string=strdup(yytext);	return FILESIZE; }
[0-9]+[.][0-9]+[.][0-9]+[.][0-9]+	 { yylval.string=strdup(yytext); return IP; }
[0-9]+  					{ yylval.number=atoi(yytext); 	return NUMBER; }
[a-zA-Z][a-zA-Z_]*   		{ yylval.string=strdup(yytext); return WORD; }
[a-zA-Z0-9\/.-]+        	{ yylval.string=strdup(yytext); return PATH; }
[a-zA-Z][a-zA-Z0-9\/:.]+	{ yylval.string=strdup(yytext); return URL; }
\n                      	/* пропускаем концы строк */;
[ \t]+                  	/* пропускаем пробелы */;
%%
