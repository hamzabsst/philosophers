NAME	= philo
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRCS	=	main.c parsing.c routine.c monitor.c \

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c philo.h
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@echo "Cleaning object files"
	@rm -f $(OBJS)

fclean: clean
	@echo "Removing excutable"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
