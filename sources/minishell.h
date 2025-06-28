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

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC
}			t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef struct s_mini
{
	char	**full_cmd;
	char	*full_path;
	int		infile;
	int		outfile;
}	t_mini;

typedef struct s_promp
{
	t_list	*cmds;
	char	**envp;
	pid_t	pid;
}			t_promp;

void	err(char *err, char *a, int exit_code);
size_t	where_is(char *arg, char needle);
void	command_check(char *arg);
void	validate_command(char *cmd, char **s_cmd);
int		open_file(char *filename, int in_out);
void	free_all(char **str);
int		ft_strcmp(const char *s1, const char *s2);
char	*get_path(char *cmd, char **env);
void	exec(char *cmd, char **env);
t_token	*ft_tokennew(char *v);
void	ft_tokenadd_back(t_token **lst, t_token *new);
void	ft_tokenclear(t_token **lst);
int		ft_tokensize(t_token *lst);
void	ft_tokenadd_front(t_token **lst, t_token *new);
void	ft_tokendel(t_token *token);
void	refine_tokens(t_token **head, t_token *(*split_func)(char *));
int		is_quotes_closed(char *s);
t_token	*split_blanks(char *s, char open, int i, int last_i);
t_token	*split_pipes1(char *s);
t_token	*split_redirs1(char *s);
t_token	*split_redirs(char *s, char quote, int i, int last_i);
t_token	*get_tokens(const char *line);
t_token	*get_classified_tokens(char	*line);
t_token	*get_unqouted_tokens(t_token *head);
char	**get_swords(char **words, char *word);
void	free_mini(void *m);
int		open_heredoc(t_token *tok);
t_list	*get_minis(t_token *tokens);
t_mini	*init_mini(void);
void	del_prompt(t_promp *prompt, void (*del)(void *));
t_promp	*get_prompt(t_list *cmds, char **env);
t_list	*get_full_minis(t_token *head, char **env);
int		count_words(char **words);
char	**get_fenv(char **e);
char	*get_expanded_data1(const char *s, int i, int last, char open);
t_token	*get_expanded_tokens(t_token *head);

#endif
