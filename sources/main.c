/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:51:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/29 11:01:31 by raydogmu         ###   ########.fr       */
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

int	main(int ac, char **argc, char **env)
{
	char	**fenv;
	char	*line;

	fenv = get_fenv(env);
	if (!ac || !argc || !fenv)
	{
		perror("fenv error");
		return (1);
	}
	while (1)
	{
		line = readline("\001\033[1;92m\002minishell > \001\033[0;39m\002");
		if (!line)
			break ;
		run(line, fenv);
		free(line);
	}
	free_all(fenv);
	return (0);
}
