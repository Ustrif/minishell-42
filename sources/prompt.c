/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:19:19 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/16 10:32:13 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_promp	*init_prompt(void)
{
	t_promp	*prompt;

	prompt = malloc(sizeof(t_promp));
	if (!prompt)
		return (NULL);
	prompt->cmds = NULL;
	prompt->envp = NULL;
	return (prompt);
}

t_promp	*get_prompt(t_list *cmds, char **env)
{
	t_promp	*prompt;

	prompt = init_prompt();
	if (!prompt)
		return (NULL);
	prompt->cmds = cmds;
	prompt->envp = env;
	return (prompt);
}

void	del_prompt(t_promp *prompt, void (*del)(void *))
{
	if (prompt->cmds)
		ft_lstclear(&prompt->cmds, del);
	free(prompt);
}
