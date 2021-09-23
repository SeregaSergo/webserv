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
 /*%token EQUAL DIRECTIVE WORD FILENAME QUOTE OBRACE EBRACE SEMICOLON LIMIT_EXCEPT;*/

%parse-param {Config *config}
%%
commands:
		| commands command
		;

command:
	   server_start 

server_start:	SERVER 
				{
					std::cout << "Server created" << std::endl;
					config->add_server();		
				}
				server_content

server_content:	OBRACE server_statements EBRACE

server_statements:	|	server_statements server_statement;

server_statement: root | error_page | location_block | listen | server_names  | client_max_body_size; 

root:	ROOT PATH SEMICOLON  
		{
		//	servers->back().setRoot($2);
		}	

error_page: 
		ERROR_PAGE error_num PATH SEMICOLON 
		{
		//	servers->back().setErrorPage($3);
		}
		|
		ERROR_PAGE error_num EQUAL PATH	SEMICOLON
		{
		//	servers->back().setErrorPage($4);
		}

error_num:
		 | error_num NUMBER

location_block:	LOCATION PATH OBRACE 
				{
					servers->back().getLocations().push_back(Location($2));
				}
				location_statements EBRACE 

location_statements: 
		| location_statements location_statement

location_statement:	autoindex | root | limit_except | error_page /*http_redirection  | default_file*/ 

limit_except: LIMIT_EXCEPT what_to_except SEMICOLON

what_to_except:	|	what_to_except HTTP_METHOD
					{
					//servers->back().getLocations().back().setLimitexcept($2);
					}

autoindex:	AUTOINDEX STATE SEMICOLON
			{
				int state = !strcmp($2, "on");
				//servers->back().getLocations().back().setAutoindex(state);
			}

listen: LISTEN what_to_listen SEMICOLON

what_to_listen: IP COLON NUMBER 
				{
					//servers->back().setListen($1,$2);
				}
				| IP 
				{
					//servers->back().setListen($1);
				}
				| NUMBER 
				{
					//servers->back().setListen($1);
				}

server_names: SERVER_NAME server_name  SEMICOLON

server_name: |	server_name PATH
				{
					servers->back().addServerName((std::string)$2);
				}

client_max_body_size:	CLIENT_MAX_BODY_SIZE NUMBER SEMICOLON
						{
							//servers->back().getLocations().back().setClientMaxBodySize($2);	
						}
%%

int yylex(void);  
