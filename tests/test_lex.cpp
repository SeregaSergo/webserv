#include "../inc/Webserv.hpp"
#include "../src/config_parser/ConfigParser.hpp"

extern "C" {
    int yylex(void);
    char * yytext;
}

void    print_token(int t)
{
    switch (t)
    {
    case ACCLOG:
        printf("ACCLOG");
        break;
    case ALLOWED_METHODS:
        printf("ALLOWED_METHODS");
        break;
    case AUTOINDEX:
        printf("AUTOINDEX");
        break;
    case CGI_ENABLED:
        printf("CGI_ENABLED");
        break;
    case CGI_TIMEOUT:
        printf("CGI_TIMEOUT");
        break;
    case CLIENT_MAX_BODY_SIZE:
        printf("C_M_B_S");
        break;
    case DAEMON:
        printf("DAEMON");
        break;
    case ERRLOG:
        printf("ERRLOG");
        break;
    case ERROR_PAGE:
        printf("ERROR_PAGE");
        break;
    case IDLE_TIMEOUT:
        printf("IDLE_TIMEOUT");
        break;
    case INDEX:
        printf("INDEX");
        break;
    case MIMETYPES:
        printf("MIMETYPES");
        break;
    case ROOT:
        printf("ROOT");
        break;
    case REDIRECTION:
        printf("REDIR");
        break;
    case SERVER:
        printf("SERVER");
        break;
    case LISTEN:
        printf("LISTEN");
        break;
    case SERVER_NAME:
        printf("SERVER_NAME");
        break;
    case LOCATION:
        printf("LOCATION");
        break;
    case URI:
        printf("URI");
        break;
    case QUOTE:
        printf("QUOTE");
        break;
    case OBRACE:
        printf("OBRACE");
        break;
    case EBRACE:
        printf("EBRACE");
        break;
    case SEMICOLON:
        printf("SEMICOLON");
        break;
    case STATE:
        printf("STATE");
        break;
    case NUMBER:
        printf("NUM");
        break;
    case WORD:
        printf("WORD");
        break;
    case PATH:
        printf("PATH");
        break;
    case URL:
        printf("URL");
        break;
    case FILESIZE:
        printf("FILESIZE");
        break;
    case HTTP_METHOD:
        printf("HTTP_METHOD");
        break;
    case IP:
        printf("IP");
        break;
    case KA_TIMEOUT:
        printf("KA_TIMEOUT");
        break;
    case KA_TIME:
        printf("KA_TIME");
        break;
    case KA_PROBES:
        printf("KA_PROBES");
        break;
    case KA_INTERVAL:
        printf("KA_INTERVAL");
        break;
    case COLON:
        printf("COLON");
        break;
    case EQUALS:
        printf("EQUALS");
        break;
    case TILDE:
        printf("TILDE");
        break;        
    default:
        printf("undef");
        break;
    }
    printf("(%s)", yytext);
    if (t == OBRACE || t == SEMICOLON)
        printf("\n");
    else
        printf(" ");
}

int main(void)
{
    int _token;

    _token = yylex();
    while (_token)
    {
        print_token(_token);
        _token = yylex();
    }
    return (0);
}