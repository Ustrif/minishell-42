/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   error.c                                                       :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: raydogmu <raydogmu@student.42istanbul.com.tr>         +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/05/22 22:59:48 by raydogmu                     #+#    #+#              */
/*   Updated: 2025/05/22 22:59:48 by raydogmu                    ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"

void	err(char *err, char *a, int exit_code)
{
	int	i;

	i = 0;
	ft_putstr_fd(err, 2);
	while (a[i] != '\0')
	{
		write(2, &a[i], 1);
		i++;
	}
	write(2, "\n", 1);
	exit(exit_code);
}

size_t	where_is(char *arg, char needle)
{
	size_t	wc;
	int		i;

	wc = 0;
	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == needle)
			wc++;
		i++;
	}
	return (wc);
}

void	command_check(char *arg)
{
	if ((ft_strlen(arg) == 0) || (where_is(arg, '/') == ft_strlen(arg)))
		err("permission denied: ", arg, 126);
	if (((where_is(arg, '.') == 1)))
		err("not enough arguments:", arg, 1);
	if (((where_is(arg, '.') == 2)))
		err("permission denied: ", arg, 126);
	if (where_is(arg, '.') == ft_strlen(arg))
		err("command not found: ", arg, 127);
	if (where_is(arg, ' ') == ft_strlen(arg))
		err("command not found: ", arg, 127);
}

void	validate_command(char *cmd, char **s_cmd)
{
	if (!s_cmd[0] || !s_cmd)
	{
		free_all(s_cmd);
		err("command not found: ", cmd, 127);
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (str1[i] != '\0' || str2[i] != '\0')
	{
		if ((str1[i] < str2[i]) || (str1[i] > str2[i]))
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}
