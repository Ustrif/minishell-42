/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:20:40 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/24 20:30:34 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}
