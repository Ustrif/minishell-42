/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 23:05:10 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/02 00:38:26 by codespace        ###   ########.fr       */
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
	new->appendfilec = NULL;
	new->heredoc = NULL;
	new->infilec = NULL;
	new->outfilec = NULL;
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
	if (mini->infile != STDIN_FILENO)
		close(mini->infile);
	if (mini->outfile != STDOUT_FILENO)
		close(mini->outfile);
	free(mini->appendfilec);
	free(mini->heredoc);
	free(mini->infilec);
	free(mini->outfilec);
	free(mini);
}
