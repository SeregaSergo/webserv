NAME =			../config_parser.cc

.PHONY: all

all: $(NAME)

$(NAME) : lexer.l YACC.y
	lex lexer.l
	yacc  -t --verbose --debug -d YACC.y -o $(NAME)
