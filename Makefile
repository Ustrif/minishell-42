SRC = sources/basics.c sources/built_in.c sources/env.c sources/error.c \
	sources/exec1.c sources/exec2.c sources/exec3.c sources/exec4.c \
	sources/execbe.c sources/exec.c sources/exit.c sources/expand1.c \
	sources/expand2.c sources/expand.c sources/export2.c sources/export.c \
	sources/export_utils.c sources/file.c sources/full_path.c sources/heredoc.c \
	sources/heredocc.c sources/main.c sources/parser1.c sources/parser.c \
	sources/pathcheck.c sources/prompt.c sources/quote.c sources/redir.c \
	sources/syntax.c sources/token2,5.c sources/token2.c sources/token3.c \
	sources/token4.c sources/token.c sources/type.c sources/unset.c \
	sources/utils.c sources/utils_re2.c sources/utils_re.c sources/utlis2.c \
	sources/parser2.c sources/exec0.c
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