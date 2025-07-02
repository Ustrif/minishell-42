/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:20:40 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/02 09:04:54 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_line(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

static void	heredoc_child(char *delim, int write_fd)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	while (1)
    {
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: here-document delimited by end-of-file\n", 2);
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write_line(line, write_fd);
		free(line);
	}
	close(write_fd);
	exit(0);
}

int	errcase(int fd)
{
	close(fd);
	return (-1);
}

int	open_heredoc(char *delim)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(fd[0]);
		heredoc_child(delim, fd[1]);
	}
	close(fd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, signal_handler);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		errcase(fd[0]);
	return (fd[0]);
}
