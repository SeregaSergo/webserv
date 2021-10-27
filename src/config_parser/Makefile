LEX_SRC = lex.yy.c
YACC_SRC = ConfigParser.cpp
YACC_HHP = $(YACC_SRC:%.cpp=%.hpp)

PATH_O = ../../obj/

LEX_OBJ = $(addprefix $(PATH_O), $(LEX_SRC:%.c=%.o))
YACC_OBJ = $(addprefix $(PATH_O), $(YACC_SRC:%.cpp=%.o))

CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -std=c++98

.PHONY: all clean

all: produce_objs

produce_objs : lexer.l YACC.y
	lex lexer.l
	yacc -d YACC.y -o $(YACC_SRC)
	clang $(CFLAGS) -c $(LEX_SRC) -o $(LEX_OBJ)
	clang++ $(CFLAGS) $(CPPFLAGS) -c $(YACC_SRC) -o $(YACC_OBJ)

clean:
	/bin/rm -f $(LEX_SRC) $(YACC_SRC) $(YACC_HHP)