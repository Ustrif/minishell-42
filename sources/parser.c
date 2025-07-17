/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 23:05:10 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/16 16:39:23 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_mini	*init_mini(void)
{
	t_mini	*new;

	new = malloc(sizeof(t_mini));
	if (!new)
		return (NULL);
	new->full_cmd = NULL;
	new->full_path = NULL;
	new->redir = NULL;
	new->infile = STDIN_FILENO;
	new->outfile = STDOUT_FILENO;
	return (new);
}

void	free_mini(void *m)
{
	t_mini	*mini;

	mini = m;
	if (!mini)
		return ;
	if (mini->full_cmd)
		free_all(mini->full_cmd);
	free(mini->full_path);
	ft_redirclear(&mini->redir);
	free(mini);
}
