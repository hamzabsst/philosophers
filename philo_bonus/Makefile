NAME	= philo_bonus
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRCS	=	main_bonus.c parsing_bonus.c routine_bonus.c monitor_bonus.c\

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c philo_bonus.h
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

bonus: all

.PHONY: all clean fclean re bonus
