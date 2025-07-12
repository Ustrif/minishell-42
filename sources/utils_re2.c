/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_re2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:30:39 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/12 13:56:45 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(char **str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	check_builtin(char *s)
{
	int	r;

	r = 0;
	if (ft_strcmp(s, "pwd") == 0)
		r = 1;
	if (ft_strcmp(s, "cd") == 0)
		r = 1;
	if (ft_strcmp(s, "exit") == 0)
		r = 1;
	if (ft_strcmp(s, "unset") == 0)
		r = 1;
	if (ft_strcmp(s, "export") == 0)
		r = 1;
	if (ft_strcmp(s, "echo") == 0)
		r = 1;
	if (ft_strcmp(s, "env") == 0)
		r = 1;
	return (r);
}

int	calculate_size(const char *colon, const char *p)
{
	int	dir_len;

	if (colon)
		dir_len = (size_t)(colon - p);
	else
		dir_len = ft_strlen(p);
	return (dir_len);
}
