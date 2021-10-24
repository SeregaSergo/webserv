%{
#include <stdio.h>
#include "../inc/Config.hpp"
#include <string.h>
#include <vector>
//DELETE THIS SHIT ON MAC
int yylex(void);  
//int yydebug=1;
#define YYSTYPE char *
extern char	*yytext;
#define YYDEBUG_LEXER_TEXT yytext
int yyparse(Config *config);
extern "C"
{
	void yyerror(Config *config, const char *str)
	{
		fprintf(stderr,"ошибка: %s\n",str);
	}
	//int yylex(); 
    int yywrap()
    {
		return 1;
    }
}

%}

%token	QUOTE OBRACE EBRACE SEMICOLON COLON HTTP_METHOD NUMBER WORD STATE  EQUAL
		SERVER LISTEN SERVER_NAME LOCATION ERROR_PAGE CLIENT_MAX_BODY_SIZE  
		IP AUTOINDEX PATH  ROOT LIMIT_EXCEPT 
		FILENAME ;

/*
** This is for using different return types (see 6.3)
** http://rus-linux.net/lib.php?name=/MyLDP/algol/lex-yacc-howto.html
*/
%union 
{
    int				number;
	long long int	long_number;
    char *			string;
}

%token <number> STATE
%token <number> NUMBER
%token <string> WORD

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
		timeout
		|
		mime_types
		|
		server
		;

error_log_file: 
		ERRLOG PATH
		{
			config->error_log = $2;
			free($2);
		}
		;

daemon:
		DAEMON
		{
			config->daemon = true;
		}
		;

timeout: type_timeout NUMBER
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
			}
		}
		;
type_timeout:
		IDLE_TIMEOUT | KA_TIMEOUT | KA_TIME | KA_PROBES | KA_INTERVAL
		;

mime_types:
		MIMETYPES OBRACE mime_type_statements EBRACE
		;
mime_type_statements:
		| mime_type_statements mime_type_statement SEMICOLON
		;
mime_type_statement:
		MIMETYPE 
		{
			config->mime_temp($1);
			free($1);
		}
		extensions
		;
extensions:
		| extensions WORD
		{
			config->mime_types.insert(std::pair<std::string, std::string>($1, config->mime_temp));
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
		| server_name FILENAME
		{
			config->servers.back()->server_names.push_back($2);
			free($2);
		}
		;

listen:
		LISTEN what_to_listen
		;
what_to_listen:
		IP ':' NUMBER 
		{
			config->servers.back()->ip = $1;
			free($1);
			config->servers.back()->port = $3;
			free($3);
		}
		|
		NUMBER 
		{
			config->servers.back()->port = $1;
			free($1);
		}
		;

access_log_file: 
		ACCLOG PATH
		{
			config->servers.back()->access_log = $2;
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
			config->servers.back()->client_max_body_size = factor * atoi($2);
			free($2);
		}
		;

root:	ROOT FILENAME
		{
			config->servers.back()->root = $2;
			free($2);
		}
		;

autoindex:	AUTOINDEX STATE
		{
			config->servers.back()->autoindex = $2;
		}
		;

error_page:
		ERROR_PAGE error_num FILENAME
		{
			std::vector<int> * ptr_vec = config->servers.back()->err_num_temp;
			std::map<int, std::string> * ptr_map = config->servers.back()->err_pages;
			for (std::vector<int>::iterator it = *ptr_vec.begin(); it != *ptr_vec.end(); ++it)
				*ptr_map.insert(std::pair<int, std::string>(*it, $3));
			free($3);
		}
		;
error_num:
		| error_num NUMBER
		{
			config->servers.back()->err_num_temp.push_back($2);
		}
		;

location_block:
		LOCATION loc_type FILENAME OBRACE 
		{
			char type = constants::loc_equal_type;
			std::vector<std::string> loc_path;
			char * token = strtok(string, ",");
			while (token != NULL) {
				loc_path.push_back(token);
				token = strtok(NULL, " ");
			}
			if (loc_type == '~')
				type = constants::loc_ext_type;
			config->servers.back()->locations.push_back(Location(type, loc_path, \
							config->servers.back()->root, config->servers.back()->autoindex));
			free($3);
		}
		location_statements EBRACE
		|
		LOCATION FILENAME OBRACE 
		{
			std::vector<std::string> loc_path;
			char * token = strtok(string, ",");
			while (token != NULL) {
				loc_path.push_back(token);
				token = strtok(NULL, " ");
			}
			config->servers.back()->locations.push_back(Location(constants::loc_partly_type, loc_path, \
							config->servers.back()->root, config->servers.back()->autoindex));
			free($2);
		}
		location_statements EBRACE
		;
loc_type:
		'=' | '~'
		{
			$$ = $1;
		}
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
		;
autoindex_loc:
		AUTOINDEX STATE
		{
			config->servers.back()->locations.back().setAutoindex($2);
		}
		;
root_loc:
		ROOT FILENAME
		{
			config->servers.back()->locations.back().setRoot($2);
			free($2);
		}
		;
index_loc:
		INDEX index_file
		;
index_file:
		| index_file FILENAME
		{
			config->servers.back()->locations.back().addIndex($2);
			free($2);
		}
		;
allowed_methods:
		ALLOWED_METHODS
		{
			config->servers.back()->locations.back().clearMethodSet();
		}
		http_methods
		;
http_methods:
		HTTP_METHOD 
		{
			config->servers.back()->locations.back().addMethod($1);
			free($1);
		}
		| http_methods HTTP_METHOD
		{
			config->servers.back()->locations.back().addMethod($2);
			free($2);
		}
		;	
http_redirection:
		REDIRECTION NUMBER URL URI
		{
			Redirect * ptr = new Redirect($2, $3, true);
			setRedir(ptr);
			free($3);
		}
		|
		REDIRECTION NUMBER URL
		{
			Redirect * ptr = new Redirect($2, $3, false);
			setRedir(ptr);
			free($3);
		}
		;
%%

int yylex(void);  
