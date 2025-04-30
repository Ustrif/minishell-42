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

int	ft_buildin(char *line)
{
	if(ft_strcmp(line, "pwd") == 0 || ft_strncmp(line, "pwd ", 4) == 0)
		return(get_pwd());
	return(0);
	
}

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
		allwords = ft_split(line, ' ');
		ft_buildin(line);
		pid = fork();
		if (pid == 0)
			exec(allwords[0], env);
		waitpid(-1, NULL, 0);
		free_all(allwords);
		free(line);
	}
	return (0);
}

// $?+$? (127+127 gibi şeyler vermeli)

/*echo "merhaba ssen"|grep x
echo "merhaba ssen" |grep x
echo merhaba ssen | grep x
*/
// git fetch --all
// ilki komut mu?
// tek tırnak kontrolü. tırnak chr de kalmalı.
// çift tırnak kontrolü. tırnaklar kalır.
// boşluğa göre ayır. tırnak yoksa.
// tırnak olmayanları pipe, heredoc ayır.
// çift tırnakta dolar varsa işlem.
// ttırnakları kaldır.

/*
'echo' -n "merhaba ssen"|grep x
"'echo'" "-n "merhaba ssen"|grep x"
"'echo'" "-n" *""merhaba ssen"" "|grep x"
"'echo'" "-n" *""merhaba ssen"" "|grep" "x"
"'echo'" "-n" *""merhaba ssen"" "|" "grep" "x"*/