NAME = ../ConfigParser.ccp

.PHONY: all

all: $(NAME)

$(NAME) : lexer.l YACC.y
	lex lexer.l
	yacc  -t --verbose -d YACC.y -o $(NAME)
