/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:51:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/02 00:51:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

void	run(char *s, char **fenv, t_env *tenv)
{
	t_promp		*prompt;
	static int	err_code = 0;
	char		*line;

	line = get_expanded_data(s, err_code);
	if (basics(line, &err_code))
		return ;
	prompt = get_full_promp(line, fenv, &err_code);
	if (!prompt)
		return ;
	prompt->tenv = tenv;
	prompt->err_code = &err_code;
	free(line);
	del_prompt(prompt, free_mini);
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130;
		write(1, "\n", 1);
		//rl_replace_line("", 0); ctrl -c ve cat ctrl -c
		rl_on_new_line();
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		/*rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();*/
		return ; // soruna sebep oluyor. ust taraf.
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
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	get_real_path(*env, fenv);
	get_env_value(env_list, NULL);
	while (1)
	{
		line = readline("\001\033[1;92m\002minishell > \001\033[0;39m\002");
		if (!line)
			break ;
		run(line, fenv, env_list);
		add_history(line);
		free(line);
	}
	rl_clear_history();
	free_all(fenv);
	return (0);
}
