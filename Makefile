# Program names
NAME =			server
NAME_D_LEX =	test_lex
NAME_CLIENT = 	cl

# Directory paths
PATH_T =		tests
PATH_I =		inc
PATH_S =		src
PATH_O =		obj
PATH_P =		src/config_parser

# Main files
MAIN =			main.cpp
MAIN_OBJ =		$(addprefix $(PATH_O)/,$(MAIN:%.cpp=%.o))

MAIN_LEX = 		test_lex.cpp
MAIN_LEX_OBJ =	$(addprefix $(PATH_O)/,$(MAIN_LEX:%.cpp=%.o))

MAIN_CLIENT =	cl.cpp
MAIN_CL_OBJ =	$(addprefix $(PATH_O)/,$(MAIN_CLIENT:%.cpp=%.o))

# Files of basic program
SRCS =			Webserv.cpp \
				Server.cpp \
				VirtServer.cpp \
				Location.cpp \
				Client.cpp \
				Logger.cpp \
				Redirect.cpp \
				Request.cpp \
				Response.cpp \
				constants.cpp \
				AMethod.cpp \
				Get.cpp \
				Post.cpp \
				Delete.cpp \
				InCGI.cpp \
				OutCGI.cpp
OBJS =			$(addprefix $(PATH_O)/,$(SRCS:%.cpp=%.o))

# Lexer files
LEX_SRC =		lexer.l
LEX_PROD =		lex.yy.c
LEX_C =			$(addprefix $(PATH_P)/, $(LEX_PROD))
LEX_OBJ =		$(addprefix $(PATH_O)/,$(LEX_PROD:%.c=%.o))

# Yacc files
YACC_SRC = 		YACC.y
YACC_PROD =		ConfigParser.cpp
YACC_CPP =		$(addprefix $(PATH_P)/, $(YACC_PROD))
YACC_HPP = 		$(YACC_CPP:%.cpp=%.hpp)
YACC_OBJ =		$(addprefix $(PATH_O)/, $(YACC_PROD:%.cpp=%.o))

CC =			clang
CXX =			clang++
CFLAGS =		-g -Wall -Wextra -Werror -MMD
CXXFLAGS =		-std=c++98
all: CMDL_FLAGS =
display: CMDL_FLAGS = -DDISPLAY_FLAG
debug: CMDL_FLAGS = -DDISPLAY_FLAG -DDEBUG_FLAG -fsanitize=address

OS := $(shell uname)
ifeq ($(OS), Linux)
    OSFLAGS = -DLINUX_COMPILATION
else
    OSFLAGS = -DMACOS_COMPILATION
endif


DEPS =			$(MAIN_OBJ:%.o=%.d) \
				$(MAIN_LEX_OBJ:%.o=%.d) \
				$(MAIN_CL_OBJ:%.o=%.d) \
				$(OBJS:%.o=%.d) \
				$(LEX_OBJ:%.o=%.d) \
				$(YACC_OBJ:%.o=%.d)

.PHONY: all client debug_lex directories  clean fclean re

all: directories $(NAME)
$(NAME): $(OBJS) $(MAIN_OBJ) $(YACC_OBJ) $(LEX_OBJ)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CMDL_FLAGS) $(OSFLAGS) $^ -o $@

display: directories $(NAME)

debug: directories $(NAME)

# Rule for testing lexer's output
debug_lex: directories $(NAME_D_LEX)
$(NAME_D_LEX): $(OBJS)  $(YACC_OBJ) $(LEX_OBJ) $(MAIN_LEX_OBJ)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(OSFLAGS) $^ -o $@

$(OBJS) $(MAIN_OBJ): $(addprefix $(PATH_O)/, %.o) : $(addprefix $(PATH_S)/, %.cpp)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CMDL_FLAGS) $(OSFLAGS) -c $< -o $@

$(MAIN_LEX_OBJ): $(addprefix $(PATH_O)/, %.o) : $(addprefix $(PATH_T)/, %.cpp)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CMDL_FLAGS) $(OSFLAGS) -c $< -o $@

$(LEX_OBJ): $(LEX_C) $(YACC_HPP)
	$(CC) $(CFLAGS) $(OSFLAGS) -c $< -o $@

$(LEX_C): $(addprefix $(PATH_P)/, $(LEX_SRC))
	flex $<
	@mv $(LEX_PROD) $(PATH_P)

$(YACC_OBJ): $(YACC_CPP)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(OSFLAGS) -c $< -o $@

$(YACC_CPP) $(YACC_HPP) &: $(addprefix $(PATH_P)/, $(YACC_SRC))
	yacc $< -d -o $(YACC_CPP)

-include $(DEPS)

# Client for sending requests / testing
client: directories $(addprefix $(PATH_T)/,$(NAME_CLIENT))
$(addprefix $(PATH_T)/,$(NAME_CLIENT)): $(MAIN_CL_OBJ)
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(OSFLAGS) $< -o $@

$(MAIN_CL_OBJ): $(addprefix $(PATH_T)/, $(MAIN_CLIENT))
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(OSFLAGS) -c $< -o $@

# Rule for creating derictory
directories:
	@mkdir -p $(PATH_O)

clean:
	/bin/rm -f  $(MAIN_OBJ) $(MAIN_LEX_OBJ) $(OBJS) $(LEX_OBJ) \
				$(YACC_OBJ) $(DEPS) $(YACC_CPP) $(YACC_HPP) $(LEX_C) \
				$(MAIN_CL_OBJ)

fclean: clean
	/bin/rm -f $(NAME) $(NAME_D_LEX) $(addprefix $(PATH_T)/, $(NAME_CLIENT))

re: fclean all
