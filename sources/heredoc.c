/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:20:40 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/02 00:31:47 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	write_line(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

int	open_heredoc(char *delim)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("minishell: here-document delimited by end-of-file\n");
			break ;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write_line(line, fd[1]);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}
