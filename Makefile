SRC = sources/*.c
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a
NAME = minishell

all: $(NAME)

$(NAME): $(SRC)
	$(MAKE) -C libft
	$(CC) $(CFLAGS) $(SRC) $(LIBFT) -lreadline -o $(NAME)
clean:
	$(MAKE) -C libft clean
	rm -rf $(NAME)

fclean: clean
	$(MAKE) -C libft fclean

re: fclean all

.PHONY: all clean fclean re