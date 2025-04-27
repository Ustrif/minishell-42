/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:02:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/04/23 16:54:58 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **argc, char **env)
{
	char	*line;
	char	**allwords;
	char	*path;

	printf("argc kullanma satiri: %s \n", argc[0]); // kullanilmiyor kaldir.
	ac++; // ac kullanilmiyor kaldir.
	while (1)
	{
		line = readline("minishell > ");
		allwords = ft_split(line, ' ');
		path = get_path(allwords[0], env);
		if (path == NULL)
			err("command not found: ", allwords[0], 127);
		printf("%s \n %s:path \n", line, path);
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