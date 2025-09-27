# Variables
CC := gcc
CFLAGS := -Wall -Wextra -Werror
#? flags to ignore invalid specifier
#CFLAGS = -Wall -Wextra -Werror -Wno-format
NAME = libftprintf.a

# SRCS
SRCS := ft_printf.c ft_base_utils.c ft_arg_utils.c

# OBJS
OBJS := $(SRCS:.c=.o)

# Rules
all: $(NAME)

$(NAME): $(OBJS) ft_printf.h
	ar -rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re