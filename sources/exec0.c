/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:50:51 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/17 18:49:04 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

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
				if (fd == -130)
					g_status = 130;
				r->heredoc_fd = fd;
			}
			r = r->next;
		}
		i++;
	}
}
