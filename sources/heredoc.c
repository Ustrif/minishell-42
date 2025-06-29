/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:20:40 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/29 12:59:16 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	write_line(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

int	open_heredoc(t_token *tok)
{
	char	*delim;
	int		fd[2];
	char	*line;

	delim = tok->next->value;
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
