%{
#include <stdio.h>
#include <stdlib.h>
#include "../../inc/Webserv.hpp"
extern char	*yytext;


extern "C"
{
	void yyerror(Config *config, const char *str)
	{
		(void)config;
		fprintf(stderr,"ошибка: %s\n",str);
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

%}

%token	ERRLOG ACCLOG DAEMON MIMETYPES ROOT ALLOWED_METHODS INDEX REDIRECTION
		AUTOINDEX SERVER LISTEN SERVER_NAME LOCATION ERROR_PAGE 
		CLIENT_MAX_BODY_SIZE URI QUOTE OBRACE EBRACE SEMICOLON COLON
		CGI_INTERPRETER CGI_TIMOUT

/*
** This is for using different return types (see 6.3)
** http://rus-linux.net/lib.php?name=/MyLDP/algol/lex-yacc-howto.html
*/

%union 
{
    int				number;
    char *			string;
	char			letter;
}

%token <number> STATE
%token <number> NUMBER
%token <string> WORD
%token <string> PATH
%token <string> URL
%token <string> FILESIZE
%token <string> HTTP_METHOD
%token <string> IP


%type <number> num_constant
%type <letter> loc_type

%token <number> IDLE_TIMEOUT
%token <number> KA_TIMEOUT
%token <number> KA_TIME
%token <number> KA_PROBES
%token <number> KA_INTERVAL
%token <number> URI_LENGTH
%token <number> REQUEST_LENGTH
%token <number> EQUALS
%token <number> TILDE

%parse-param {Config *config}
%%
commands:
		| commands command SEMICOLON
		;
command:
		error_log_file
		|
		daemon
		|
		num_constants
		|
		mime_types
		|
		server
		;

error_log_file: 
		ERRLOG PATH
		{
			config->err_log = $2;
			free($2);
		}
		;

daemon:
		DAEMON
		{
			config->daemon = true;
		}
		;

num_constants: num_constant NUMBER
		{
			switch ($1)
			{
			case IDLE_TIMEOUT:
				config->timeout_idle = $2;
				break;
			case KA_TIMEOUT:
				config->timeout_ka = $2;
				break;
			case KA_TIME:
				config->keepalive_time = $2;
				break;
			case KA_PROBES:
				config->num_probes = $2;
				break;
			case KA_INTERVAL:
				config->keepalive_intvl = $2;
				break;
			case URI_LENGTH:
				config->limit_uri_length = $2;
				break;
			case REQUEST_LENGTH:
				config->limit_headers_length = $2;
				if (config->incoming_buffer < $2)
					config->incoming_buffer = $2;
				break;
			}
		}
		;
num_constant:
		IDLE_TIMEOUT		{ $$ = IDLE_TIMEOUT; }
		| KA_TIMEOUT 		{ $$ = KA_TIMEOUT; }
		| KA_TIME 			{ $$ = KA_TIME; }
		| KA_PROBES 		{ $$ = KA_PROBES; }
		| KA_INTERVAL		{ $$ = KA_INTERVAL; }
		| URI_LENGTH		{ $$ = URI_LENGTH; }
		| REQUEST_LENGTH	{ $$ = REQUEST_LENGTH; }
		;

mime_types:
		MIMETYPES OBRACE mime_type_statements EBRACE
		;
mime_type_statements:
		| mime_type_statements mime_type_statement SEMICOLON
		;
mime_type_statement:
		PATH 
		{
			config->mime_temp = std::string($1);
			free($1);
		}
		extensions
		;
extensions:
		| extensions WORD
		{
			config->mime_types.insert(std::pair<std::string, std::string>($2, config->mime_temp));
			free($2);
		}
		;

server:	SERVER OBRACE
		{
			config->servers.push_back(ConfigServ());		
		}
		server_statements EBRACE
		;
server_statements:
		| server_statements server_statement SEMICOLON
		;
server_statement:
		server_names
		|
		listen
		|
		access_log_file
		|
		client_max_body_size
		|
		root
		|
		autoindex
		|
		error_page
		|
		location_block
		;

server_names:
		SERVER_NAME server_name
		;
server_name:
		| server_name PATH
		{
			config->servers.back().server_names.push_back($2);
			free($2);
		}
		;

listen:
		LISTEN what_to_listen
		;
what_to_listen:
		IP COLON NUMBER 
		{
			config->servers.back().ip = $1;
			free($1);
			config->servers.back().port = $3;
		}
		|
		NUMBER 
		{
			config->servers.back().port = $1;
		}
		;

access_log_file: 
		ACCLOG PATH
		{
			config->servers.back().acc_log = $2;
			free($2);
		}
		;

client_max_body_size:
		CLIENT_MAX_BODY_SIZE FILESIZE
		{
			int l = strlen($2);
			int factor = 1;
			switch ($2[l - 1])
			{
			case 'G':
				factor *= 1024;
			case 'M':
				factor *= 1024;
			case 'K':
				factor *= 1024;
			}
			$2[l - 1] = 0;
			if (config->servers.back().locations.size() == 0)
				config->servers.back().client_max_body_size = factor * atoi($2);
			else
				config->servers.back().locations.back().setMaxBodySize(factor * atoi($2));
			free($2);
		}
		;

root:	ROOT PATH
		{
			config->servers.back().root = $2;
			free($2);
		}
		;

autoindex:	AUTOINDEX STATE
		{
			config->servers.back().autoindex = $2;
		}
		;

error_page:
		ERROR_PAGE error_num PATH
		{
			std::vector<int> * ptr_vec = &config->servers.back().err_num_temp;
			std::map<int, std::string> * ptr_map = &config->servers.back().err_pages;
			for (std::vector<int>::iterator it = (*ptr_vec).begin(); it != (*ptr_vec).end(); ++it)
				(*ptr_map).insert(std::pair<int, std::string>(*it, $3));
			free($3);
		}
		;
error_num:
		| error_num NUMBER
		{
			config->servers.back().err_num_temp.push_back($2);
		}
		;

location_block:
		LOCATION loc_type PATH OBRACE 
		{
			char type = $2;
			std::vector<std::string> loc_path;
			if (type == location::pathType::extention)
			{
				char * token = strtok($3, "/");
				while (token != NULL) {
					loc_path.push_back(token);
					token = strtok(NULL, " ");
				}
			}
			else
				loc_path.push_back($3);
			config->servers.back().locations.push_back(Location(type, loc_path, \
							config->servers.back().root, config->servers.back().autoindex, \
							config->servers.back().client_max_body_size));
			free($3);
		}
		location_statements EBRACE
		|
		LOCATION TILDE WORD OBRACE 
		{
			std::vector<std::string> loc_path;
			loc_path.push_back($3);
			config->servers.back().locations.push_back(Location(location::pathType::extention, loc_path, \
							config->servers.back().root, config->servers.back().autoindex, \
							config->servers.back().client_max_body_size));
			free($3);
		}
		location_statements EBRACE
loc_type:
		EQUALS	{ $$ = location::pathType::equal; }
		| TILDE	{ $$ = location::pathType::extention; }
		|		{ $$ = location::pathType::partial; }
		;
location_statements: 
		| location_statements location_statement SEMICOLON
		;
location_statement:
		autoindex_loc
		|
		root_loc
		|
		allowed_methods
		|
		http_redirection
		|
		index_loc
		|
		client_max_body_size
		|
		cgi_interpreter
		|
		cgi_timout
		;
autoindex_loc:
		AUTOINDEX STATE
		{
			config->servers.back().locations.back().setAutoindex($2);
		}
		;
root_loc:
		ROOT PATH
		{
			config->servers.back().locations.back().setRoot($2);
			free($2);
		}
		;
index_loc:
		INDEX index_file
		;
index_file:
		| index_file PATH
		{
			config->servers.back().locations.back().addIndex($2);
			free($2);
		}
		;
allowed_methods:
		ALLOWED_METHODS
		{
			config->servers.back().locations.back().clearMethodSet();
		}
		http_methods
		;
http_methods:
		HTTP_METHOD 
		{
			config->servers.back().locations.back().addMethod($1);
			free($1);
		}
		| http_methods HTTP_METHOD
		{
			config->servers.back().locations.back().addMethod($2);
			free($2);
		}
		;	
http_redirection:
		REDIRECTION NUMBER URL URI
		{
			Redirect * ptr = new Redirect($2, $3, true);
			config->servers.back().locations.back().setRedir(ptr);
			free($3);
		}
		|
		REDIRECTION NUMBER URL
		{
			Redirect * ptr = new Redirect($2, $3, false);
			config->servers.back().locations.back().setRedir(ptr);
			free($3);
		}
		;
cgi_interpreter:
		CGI_INTERPRETER PATH
		{
			config->servers.back().locations.back().setCgiInterpreter($2);
			free($2);
		}
		|
		CGI_INTERPRETER WORD
		{
			config->servers.back().locations.back().setCgiInterpreter($2);
			free($2);
		}
		;
cgi_timout:
		CGI_TIMOUT NUMBER
		{
			config->servers.back().locations.back().setCgiTimout($2);
		}
		;
%%

int yylex(void);  
