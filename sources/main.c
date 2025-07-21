/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:51:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/21 11:45:08 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

int	g_status = 0;

void	set_datas(int *e)
{
	if (g_status == 130)
	{
		*e = 130;
		g_status = 0;
	}
}

int	run(char *s, char **fenv, t_env *tenv)
{
	t_promp			*prompt;
	static int		err_code = 0;
	char			*line;
	int				code;

	set_datas(&err_code);
	line = get_expanded_data(s, err_code);
	if (basics(line, &err_code))
		return (free_return(line, 0));
	add_history(s);
	signal(SIGINT, SIG_IGN);
	prompt = get_full_promp(line, fenv, &err_code, tenv);
	if (!prompt)
		return (free_return(line, 1));
	prompt->tenv = tenv;
	prompt->err_code = &err_code;
	code = execute_pipeline(prompt);
	if (g_status == 130)
		for_heredoc(prompt->cmds);
	free(line);
	del_prompt(prompt, free_mini);
	return (code);
}

int	pre_run(char *s, char **env, t_env *tenv)
{
	struct termios	saved;
	int				code;

	if (!s)
		print_error("memory error", -1);
	tcgetattr(STDIN_FILENO, &saved);
	code = run(s, env, tenv);
	tcsetattr(STDIN_FILENO, TCSANOW, &saved);
	return (code);
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	main(int ac, char **argc, char **env)
{
	char	*line;
	t_env	*env_list;
	int		code;

	if (!ac || !argc)
		return (print_error("argc error", 1));
	code = 0;
	env_list = init_env(env);
	if (!env_list)
		return (print_error("env_list error", 1));
	get_env_value(env_list, NULL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		signal(SIGINT, signal_handler);
		line = readline("\001\033[1;92m\002minishell > \001\033[0;39m\002");
		if (!line)
			line = ft_strdup("exit");
		code = pre_run(line, env, env_list);
		free(line);
		if (code < 0)
			break ;
	}
	rl_clear_history();
	return (get_exit_code(code, env_list));
}
