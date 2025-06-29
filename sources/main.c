/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:51:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/29 13:22:48 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

void	run(char *s, char **fenv)
{
	t_promp	*prompt;

	if (basics(s))
		return ;
	prompt = get_full_promp(s, fenv);
	if (!prompt)
		return ;
	execute_cmds(prompt);
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

	fenv = get_fenv(env);
	if (!ac || !argc || !fenv)
		return (print_error("minishell: fenv memory alloc err", 1));
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	while (1)
	{
		line = readline("\001\033[1;92m\002minishell > \001\033[0;39m\002");
		if (!line)
			break ;
		run(line, fenv);
		add_history(line);
		free(line);
	}
	rl_clear_history();
	free_all(fenv);
	return (0);
}
