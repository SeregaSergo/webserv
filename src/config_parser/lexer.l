%{
	#include <stdio.h>
	//DELETE THIS SHIT ON MAC
	//IM SERIOUS
	typedef struct Config Config;
	#include "ConfigParser.hpp"
	int yylex(void);
%}

%option nounput
%option noinput

%%

access_log					return ACCLOG;
allowed_methods				return ALLOWED_METHODS;
autoindex					return AUTOINDEX;
cgi_enabled					return CGI_ENABLED;
cgi_timeout					return CGI_TIMEOUT;
client_max_body_size		return CLIENT_MAX_BODY_SIZE;
daemon_mode					return DAEMON;
error_log					return ERRLOG;
error_page					return ERROR_PAGE;
idle_timeout				return IDLE_TIMEOUT;
index						return INDEX;
keepalive_timeout			return KA_TIMEOUT;
limit_request_entity		return REQUEST_LENGTH;
limit_uri_length			return URI_LENGTH;
listen						return LISTEN;
location					return LOCATION;
redirect					return REDIRECTION; 
root						return ROOT;
server						return SERVER;
server_name					return SERVER_NAME;
tcp_keepalive_time			return KA_TIME;
tcp_keepalive_probes		return KA_PROBES;
tcp_keepalive_intvl			return KA_INTERVAL;
types						return MIMETYPES;
$uri						return URI;
#							;
\"                      	return QUOTE;
\{                      	return OBRACE;
\}                      	return EBRACE;
;                       	return SEMICOLON;
:                       	return COLON;
=							return EQUALS;
~							return TILDE;
\n                      	/* пропускаем концы строк */;
[ \t]+                  	/* пропускаем пробелы */;
on|off						{ yylval.number=!strcmp(yytext,"on"); return STATE; }
GET|POST|DELETE				{ yylval.string=strdup(yytext); return HTTP_METHOD; }	
[0-9]+(B|K|M|G)				{ yylval.string=strdup(yytext);	return FILESIZE; }
[0-9]+[.][0-9]+[.][0-9]+[.][0-9]+	 { yylval.string=strdup(yytext); return IP; }
[0-9]+  					{ yylval.number=atoi(yytext); 	return NUMBER; }
[a-zA-Z][a-zA-Z0-9_]*   	{ yylval.string=strdup(yytext); return WORD; }
[a-zA-Z0-9\/._-]+        	{ yylval.string=strdup(yytext); return PATH; }
[a-zA-Z][a-zA-Z0-9\/:.]+	{ yylval.string=strdup(yytext); return URL; }
%%
