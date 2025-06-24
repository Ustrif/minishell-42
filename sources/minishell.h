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

typedef struct s_flags
{
    int forw_hdoc;
    int back_hdoc;
    int in_redir;
    int out_redir;
    int pipe;

}   t_flags;

typedef struct s_env {
	char		*key;
	char		*value;
	struct s_env	*next;
} t_env;

void	err(char *err, char *a, int exit_code);
size_t	where_is(char *arg, char needle);
void	command_check(char *arg);
void	validate_command(char *cmd, char **s_cmd);
int	open_file(char *filename, int in_out);
void	free_all(char **str);
int	ft_strcmp(const char *s1, const char *s2);
char	*get_path(char *cmd, char **env);
void	exec(char *cmd, char **env);
int   get_pwd();
int	get_cd(char **args);
int	ft_echo(char **args);
int ft_env(char **args, t_env *env_list);
t_env	*create_env(const char *env_str);
void	env_add_node(t_env **env_list, t_env *new_node);
int		command_export(t_env **env_list, char **args);
int	ft_cd(char **args);
int	get_cd(char **args);
int command_unset(char **args, t_env **env_list);
int is_valid_env_key(const char *key);
int	ft_exit(char **args);
char *ft_strndup(const char *s, size_t n) ;
t_env *find_env(t_env *env, const char *key);
t_env *create_env(const char *env_str);
void env_add_node(t_env **env_list, t_env *new_node);

#endif
