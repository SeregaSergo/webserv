NAME =			server

PATH_T =		./tests
PATH_I =		./inc
PATH_S =		./src
PATH_O =		./obj
PATH_P =		./src/config_parser

SRCS =			main.cpp
OBJS =			$(SRCS:%.cpp=%.o)

CC =			clang++
CFLAGS =		-Wall -Wextra -Werror -std=c++98

DEPS =			$(OBJS:.o=.d)

.PHONY: all clean fclean re

all: config_parser $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(addprefix $(PATH_O)/, $<) -o $(NAME)

%.o: $(addprefix $(PATH_S)/, %.cpp) | obj_direction
	$(CC) $(CFLAGS) -MMD -c -o $(addprefix $(PATH_O)/, $@) $<

obj_direction:
	@mkdir -p $(PATH_O)

config_parser:
	make -C $(PATH_P)

-include $(DEPS)

clean:
	/bin/rm -f $(OBJS) $(DEPS)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
