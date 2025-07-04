/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:51:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/04 17:39:07 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>

int	g_status = 0;

void	run(char *s, char **fenv, t_env *tenv)
{
	t_promp		*prompt;
	static int	err_code = 0;
	char		*line;
	struct termios saved;

	if (g_status == 130)
	{
		err_code = 130;
		g_status = 0;
	}
    tcgetattr(STDIN_FILENO, &saved);
	line = get_expanded_data(s, err_code);
	if (basics(line, &err_code))
		return ;
	add_history(s);
	signal(SIGINT, SIG_IGN);
	prompt = get_full_promp(line, fenv, &err_code);
	if (!prompt)
		return ;
	prompt->tenv = tenv;
	prompt->err_code = &err_code;
	execute_pipeline(prompt);
	if (g_status == 130 && (err_code == 0 || err_code == 130) && !is_builtin(((t_mini*)prompt->cmds->content)->full_cmd))
		write(1, "\n", 1);
	free(line);
	del_prompt(prompt, free_mini);
	tcsetattr(STDIN_FILENO, TCSANOW, &saved);
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
	char	**fenv;
	char	*line;
	t_env	*env_list;

	fenv = get_fenv(env);
	if (!ac || !argc || !fenv)
		return (print_error("minishell: fenv memory alloc err", 1));
	env_list = init_env(env);
	if (!env_list)
		return (free(fenv), 1);
	get_real_path(*env, fenv);
	get_env_value(env_list, NULL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		signal(SIGINT, signal_handler);
		line = readline("\001\033[1;92m\002minishell > \001\033[0;39m\002");
		if (!line)
			break ;
		run(line, fenv, env_list);
		free(line);
	}
	rl_clear_history();
	free_all(fenv);
	return (0);
}
