/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:51:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/28 19:49:09 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

void	run(char *s, char **fenv)
{
	t_promp	*prompt;

	prompt = get_full_promp(s, fenv);
	if (!prompt)
	{
		perror("prompt error");
		return ;
	}
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
		line = readline("minishell > ");
		if (!line)
			break ;
		run(line, fenv);
		free(line);
	}
	free_all(fenv);
	return (0);
}
