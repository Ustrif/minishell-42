/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:02:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/04/30 21:07:08 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

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
		add_history(line);// BUNU KALDIRMAYI UNUTMA	
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
}


/*
int	main(int ac, char **argc, char **env)
{
	char	*line;
	char	**allwords;
	int		pid;

	printf("argc kullanma satiri: %s \n", argc[0]); // kullanilmiyor kaldir.
	ac++; // ac kullanilmiyor kaldir.
	while (1)
	{
		line = readline("minishell > ");
		if (!line)
			break;
		add_history(line);
		allwords = ft_split(line, ' ');
		if(ft_buildin(allwords, env) == 0)
		{
			pid = fork();
			if (pid == 0)
				exec(allwords[0], env);
			waitpid(-1, NULL, 0);			
		}
		free_all(allwords);
		free(line);
	}
	return (0);
}*/
#include <readline/readline.h>
#include <readline/history.h>

/*
void	free_2d_array(char **arr)
{
	int i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}*/


// $?+$? (127+127 gibi şeyler vermeli)

/*echo "merhaba ssen"|grep x
echo "merhaba ssen" |grep x
echo merhaba ssen | grep x
*/
// git fetch --all
// ilki komut mu?
// tek tırnak kontrolü. tırnak chr de kalmalı.
// 