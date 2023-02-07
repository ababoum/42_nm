F_NONE			= \033[37;0m
F_BOLD			= \033[1m
F_ORANGE		= \033[38m
F_RED			= \033[31m
F_YELLOW		= \033[33m
F_GREEN			= \033[32m
F_CYAN			= \033[36m
F_BLUE			= \033[34m

CC			= @gcc

CFLAGS		= -Wall -Wextra -Werror -g

NAME		= ft_nm

SRCS		= srcs/main.c \
				srcs/utils.c

INCLUDES	= inc/nm.h

OBJ			= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(INCLUDES)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(F_GREEN)$(F_BOLD) $(NAME) executable is compiled and ready.$(F_NONE)"



clean:
	@rm -rf $(OBJ)
	@echo "$(F_CYAN)$(F_BOLD) .o files successfully deleted.$(F_NONE)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(F_CYAN)$(F_BOLD) $(NAME) executable(s) successfully deleted.$(F_NONE)"

re: fclean all

.PHONY: all clean fclean re