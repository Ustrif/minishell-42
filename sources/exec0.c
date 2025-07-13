/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:50:51 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/13 10:50:57 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	preprocess_heredocs(t_mini **arr, int count)
{
	int		fd;
	int		i;
	t_redir	*r;

	i = 0;
	fd = 0;
	while (i < count)
	{
		r = arr[i]->redir;
		while (r)
		{
			if (r->type == R_HEREDOC)
			{
				fd = open_heredoc(r->target);
				r->heredoc_fd = fd;
			}
			r = r->next;
		}
		i++;
	}
}
