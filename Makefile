NAME =			server

PATH_T =		./tests
PATH_I =		./inc
PATH_S =		./src
PATH_O =		./obj
PATH_P =		./src/config_parser

SRCS =			main.cpp \
				Webserv.cpp \
				Server.cpp \
				VirtServer.cpp \
				Location.cpp \
				Client.cpp \
				Logger.cpp \
				Redirect.cpp

OBJS =			$(addprefix $(PATH_O)/, *.o)

CC =			clang++
CFLAGS =		-Wall -Wextra -Werror -std=c++98

DEPS =			$(addprefix $(PATH_O)/, *.d)

.PHONY: all clean fclean re

all: config_parser $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: $(addprefix $(PATH_S)/, %.cpp) | obj_direction
	$(CC) $(CFLAGS) -MMD -c -o $(addprefix $(PATH_O)/, $@) $<

obj_direction:
	@mkdir -p $(PATH_O)

config_parser:
	make -C $(PATH_P)

-include $(DEPS)

clean:
	/bin/rm -f $(OBJS) $(DEPS)
	make clean -C $(PATH_P)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
