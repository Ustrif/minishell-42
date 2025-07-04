/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:05:47 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/04 18:06:16 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_line(const char *s, int fd)
{
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

static void	heredoc_child(const char *delim, int write_fd)
{
	char	*line;
	char	*err;

	err = "minishell: here-document delimited by end-of-file\n";
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd(err, 2);
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

static void	sigint_exit(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		exit(130);
	}
}

static void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_exit;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESETHAND;
	sigaction(SIGINT, &sa, NULL);
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
		return (err_case(fd[0], fd[1]));
	if (pid == 0)
	{
		setup_heredoc_signals();
		close(fd[0]);
		heredoc_child(delim, fd[1]);
	}
	close(fd[1]);
	waitpid(pid, &status, 0);
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		close(fd[0]);
		return (-130);
	}
	return (fd[0]);
}
