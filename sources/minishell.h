/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:58:43 by beinan            #+#    #+#             */
/*   Updated: 2025/04/19 14:58:43 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
# define EXEC 1
# define REDIR 2
# define PIPE 3
//# define LIST 4
//# define BACK 5

typedef struct s_pipe
{
    t_token_type    type;
    char            *left;
    char            *right;
    struct s_pipe   *next;
}               t_pipe;

typedef enum e_token_type 
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC
}           t_token_type;
*/
typedef struct s_token
{
    char            *value;
    struct s_token  *next;
}               t_token;

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_flags
{
    int forw_hdoc;
    int back_hdoc;
    int in_redir;
    int out_redir;
    int pipe;

}   t_flags;

void	err(char *err, char *a, int exit_code);
size_t	where_is(char *arg, char needle);
void	command_check(char *arg);
void	validate_command(char *cmd, char **s_cmd);
int	open_file(char *filename, int in_out);
void	free_all(char **str);
int	ft_strcmp(const char *s1, const char *s2);
char	*get_path(char *cmd, char **env);
void	exec(char *cmd, char **env);

t_token *ft_tokennew(char *v);
void    ft_tokenadd_back(t_token **lst, t_token *new);
void	ft_tokenclear(t_token **lst);
int	    ft_tokensize(t_token *lst);
void	ft_tokenadd_front(t_token **lst, t_token *new);
void    ft_tokendel(t_token *token);
void    refine_tokens(t_token **head, t_token *(*split_func)(char *));
int     is_quotes_closed(char *s);
t_token	*split_blanks(char *s, char open, int i, int last_i);
t_token	*split_pipes1(char *s);
t_token	*split_redirs1(char *s);
t_token	*split_redirs(char *s, char quote, int i, int last_i);

#endif
