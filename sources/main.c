/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:02:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/04/30 19:10:52 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **argc, char **env)
{
	char	*line;
	char	**allwords;

	if (!ac && !argc)
		return (0);
	while (1)
	{
		line = readline("minishell > ");
		allwords = ft_split(line, ' ');
		int pid = fork();
		if (pid == 0)
			exec(allwords[0], env);
		waitpid(-1, NULL, 0);
		free_all(allwords);
		free(line);
	}
	return (0);
}

/*echo "merhaba ssen"|grep x
echo "merhaba ssen" |grep x
echo merhaba ssen | grep x
*/

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