/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:22:38 by beinan            #+#    #+#             */
/*   Updated: 2025/04/30 21:06:59 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int  get_pwd()
{
    char buffer[1024];

    if (getcwd(buffer, sizeof(buffer)) != NULL)
    {
        //write(1, "dir:  ", 7);
        write(1, &buffer, ft_strlen(buffer));
        write(1, "\n", 1);
        return (0);
    }
    else
    {
        //perror("getcwd error");
        return (1);
    }
    
}

int	get_cd(char **args)
{
    if (!args[1])
    {
        write(2, "cd: missing argument\n", 22);
        return (1);
    }
    if (chdir(args[1]) != 0)
    {
        perror("cd");
        return (1);
    }
    return (0);
}

int	is_n_flag(char *arg)
{
	int i = 1;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(char **args)
{
	int i = 1;
	int newline = 1;

	while (args[i] && is_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}

	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline == 1)
		write(1, "\n", 1);
	return (0);
}

int ft_env(char **args, t_env *env_list)
{
	(void)args;
	while (env_list)
	{
		if (env_list->value)
		{
			write(1, env_list->key, ft_strlen(env_list->key));
			write(1, "=", 1);
			write(1, env_list->value, ft_strlen(env_list->value));
			write(1, "\n", 1);
		}
		env_list = env_list->next;
	}
	return (0);
}
