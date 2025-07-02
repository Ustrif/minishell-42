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
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <sys/stat.h>

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC
}			t_token_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef enum e_redir_type
{
    R_IN,
    R_OUT,
    R_APPEND,
    R_HEREDOC
} t_redir_type;

typedef struct s_redir
{
    t_redir_type     type;
    char            *target;
    struct s_redir  *next;
} t_redir;

typedef struct s_mini
{
	char	**full_cmd;
	char	*full_path;
	t_redir	*redir;
	int		infile;
	int		outfile;
}	t_mini;

typedef struct s_promp
{
	t_list	*cmds;
	char	**envp;
	t_env	*tenv;
	int		*err_code;
	pid_t	pid;
}			t_promp;

void	err(char *err, char *a, int exit_code);
size_t	where_is(char *arg, char needle);
void	command_check(char *arg);
void	validate_command(char *cmd, char **s_cmd);
int		open_file(char	*filename, int in_out);
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
int		open_heredoc(char *delim);
t_list	*get_minis(t_token *tokens);
t_mini	*init_mini(void);
void	del_prompt(t_promp *prompt, void (*del)(void *));
t_promp	*get_prompt(t_list *cmds, char **env);
int		get_pwd();
int		get_cd(char **args);
void	child_process(t_promp *prompt, t_mini *mini, int **pipes, int i, int cmd_count);
int		ft_echo(char **args);
int		ft_env(char **args, t_env *env_list);
t_env	*create_env(const char *env_str);
void	env_add_node(t_env **env_list, t_env *new_node);
int		command_export(t_env **env_list, char **args);
int		ft_cd(char **args);
int		get_cd(char **args);
int		command_unset(char **args, t_env **env_list);
int		is_valid_env_key(const char *key);
int		ft_exit(char **args);
char	*ft_strndup(const char *s, size_t n);
t_env	*find_env(t_env *env, const char *key);
t_env	*create_env(const char *env_str);
void	env_add_node(t_env **env_list, t_env *new_node);
int		process_export_key_only(t_env **env_list, const char *arg);
int		handle_export_error(char *key, char *value);
int		update_existing_env(t_env *found, char *key, char *value);
int		create_new_env_node(t_env **env_list, char *key, char *value);
int		process_export_with_value(t_env **env_list, const char *arg);
int		count_env_nodes(t_env *env_list);
void	swap_env_nodes(t_env **env_array, int i, int j);
t_list	*get_full_minis(t_token *head, char **env);
int		count_words(char **words);
char	**get_fenv(char **e);
char	*get_expanded_data1(const char *s, int i, int last, char open);
void	execute_cmds(t_promp *prompt);
t_promp	*get_full_promp(char *s, char **env, int *err_code);
char	*get_scode_data1(const char *s, int i, char open, char *num);
int		basics(char *s, int *err_code);
int		print_error(char *s, int i);
char	*get_real_path(char *s, char **env);
char	*get_env_value(t_env *env_list, const char *key);
int		check_syntax(t_token *head);
t_env	*init_env(char **envp);
char	*get_expanded_data(const char *s, int err_code);
void	execute_pipeline(t_promp *promp);
t_redir	*ft_redirnnew(char *v, t_redir_type type);
void	ft_rediradd_back(t_redir **lst, t_redir *new);
void	ft_redirclear(t_redir **lst);
int		is_builtin(char **args);
int		exec_builtin(t_mini *mini, t_env **env_list);

#endif
