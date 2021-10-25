NAME = ../config_parser.cc

.PHONY: all

all: $(NAME)

$(NAME) : lexer.l YACC.y
	lex lexer.l
	yacc  -t --verbose -d YACC.y -o $(NAME)
