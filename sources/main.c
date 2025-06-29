/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:02:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/29 17:10:05 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*node;

	env_list = NULL;
	while (*envp)
	{
		node = create_env(*envp);
		env_add_node(&env_list, node);
		envp++;
	}
	return (env_list);
}

int	ft_buildin(char **args, t_env **env_list)
{
	if (!args || !args[0])
		return (1);
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (get_pwd(args));
	else if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args));
	else if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(args, *env_list));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (get_cd(args));
	else if (ft_strcmp(args[0], "export") == 0)
		return (command_export(env_list, args));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (command_unset(args, env_list));
	else if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(args));
	return (-1);
}
/*
int	main(int ac, char **argc, char **envp)
{
	char	*line;
	char	**allwords;
	int		pid;
	t_env	*env_list = init_env(envp);

	(void)ac;
	(void)argc;
	while (1)
	{
		line = readline("minishell > ");
		if (!line)
			break;
		allwords = ft_split(line, ' ');
		if (ft_buildin(allwords, &env_list) == -1)
		{
			pid = fork();
			if (pid == 0)
	exec(allwords[0], envp); //BURAYA DİĞER FONKSİYON GELECEK BU KISIM KALKACAK
			waitpid(pid, NULL, 0);
		}
		free_all(allwords);
		free(line);
	}
	return (0);
}*/
