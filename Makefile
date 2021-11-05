# Program names
NAME =			server
NAME_D_LEX =	test_lex

# Directory paths
PATH_T =		./tests
PATH_I =		./inc
PATH_S =		./src
PATH_O =		./obj
PATH_P =		./src/config_parser

# Main files
MAIN =			main.cpp
MAIN_OBJ =		$(addprefix $(PATH_O)/,$(MAIN:%.cpp=%.o))

MAIN_LEX = 		test_lex.cpp
MAIN_LEX_OBJ =	$(addprefix $(PATH_O)/,$(MAIN_LEX:%.cpp=%.o))

# Files of basic program
SRCS =			Webserv.cpp \
				Server.cpp \
				VirtServer.cpp \
				Location.cpp \
				Client.cpp \
				Logger.cpp \
				Redirect.cpp \
				constants.cpp
OBJS =			$(addprefix $(PATH_O)/,$(SRCS:%.cpp=%.o))

# Lexer files
LEX_SRC =		lexer.l
LEX_C =			lex.yy.c
LEX_OBJ =		$(addprefix $(PATH_O)/,$(SRCS:%.c=%.o))

# Yacc files
YACC_SRC = 		YACC.y
YACC_CPP =		ConfigParser.cpp
YACC_HHP = 		$(YACC_SRC:%.cpp=%.hpp)
YACC_OBJ =		$(addprefix $(PATH_O), $(YACC_SRC:%.cpp=%.o))

CC =			clang
CXX =			clang++
CFLAGS =		-Wall -Wextra -Werror -DLINUX_COMPILATION 
CXXFLAGS =		-std=c++98

DEPS =			$(MAIN_OBJ:%.o=%.d) \
				$(MAIN_LEX_OBJ:%.o=%.d) \
				$(OBJS:%.o=%.d) \
				$(LEX_OBJ:%.o=%.d) \
				$(YAC_OBJ:%.o=%.d)

.PHONY: all debug_lex clean fclean re

all: $(NAME)
$(NAME): $(OBJS) $(MAIN_OBJ) $(LEX_OBJ) $(YAC_OBJ)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $^ -o $@


debug_lex: $(NAME_D_LEX)
$(NAME_D_LEX): $(OBJS) $(MAIN_LEX_OBJ) $(LEX_OBJ) $(YAC_OBJ)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $^ -o $@


$(OBJS) $(MAIN_OBJ): $(addprefix $(PATH_S)/, $(@F:%.o=%.cpp))
	$(CXX) $(CFLAGS) $(CXXFLAGS) -MMD -c $< -o $@

$(MAIN_LEX_OBJ): $(addprefix $(PATH_T)/, $(@F:%.o=%.cpp))
	$(CXX) $(CFLAGS) $(CXXFLAGS) -MMD -c $< -o $@

$(LEX_OBJ): $(addprefix $(PATH_P)/, $(LEX_C))
	$(CC) $(CFLAGS) -MMD -c $< -o $@

%$(LEX_C): $(addprefix $(PATH_P)/, $(LEX_SRC))
	lex $< -o $@

$(YACC_OBJ): $(addprefix $(PATH_P)/, $(YACC_CPP))
	$(CXX) $(CFLAGS) (CXXFLAGS) -MMD -c $< -o $@

%$(YACC_CPP): $(addprefix $(PATH_P)/, $(YACC_SRC))
	yacc $< -d -o $@

-include $(DEPS)

clean:
	/bin/rm -f  $(MAIN_OBJ) $(MAIN_LEX_OBJ) $(OBJS) \
				$(LEX_OBJ) $(YAC_OBJ) $(DEPS) \
				$(addprefix $(PATH_P), $(YACC_CPP)) \
				$(addprefix $(PATH_P), $(YACC_HPP)) \
				$(addprefix $(PATH_P), $(LEX_C))

fclean: clean
	/bin/rm -f $(NAME) $(NAME_D_LEX)

re: fclean all
