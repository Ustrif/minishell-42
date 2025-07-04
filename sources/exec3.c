/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:22:07 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/04 18:25:49 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	do_fd_job(t_redir *r)
{
	int	fd;

	fd = -1;
	if (r->type == R_IN)
		fd = open(r->target, O_RDONLY);
	else if (r->type == R_HEREDOC)
		fd = open_heredoc(r->target);
	else if (r->type == R_OUT)
		fd = open(r->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else
		fd = open(r->target, O_CREAT | O_APPEND | O_WRONLY, 0644);
	return (fd);
}

int	setup_redirections(t_mini *cmd)
{
	t_redir	*r;
	int		fd;

	r = cmd->redir;
	while (r)
	{
		fd = do_fd_job(r);
		if (fd == -130)
			return (-130);
		if (fd < 0)
		{
			perror(r->target);
			return (1);
		}
		if (r->type == R_IN || r->type == R_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		r = r->next;
	}
	return (0);
}
