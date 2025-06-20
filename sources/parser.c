/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 23:05:10 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/20 09:40:33 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_mini	*init_mini(void)
{
	t_mini	*new;

	new = malloc(sizeof(t_mini));
	if (!new)
		return (NULL);
	new->full_cmd = NULL;
	new->full_path = NULL;
	new->infile = STDIN_FILENO;
	new->outfile = STDOUT_FILENO;
	return (new);
}

int	main(void)
{
	t_token	*head;
	t_token	*temp;
	char	*line;

	line = "e'c'ho \"hello      there\" how are 'you 'doing? $USER |wc -l >outfile";
	//line = "'echo' -n \"merhaba ssen\"|grep x";
	//line = "echo << eof | grep 'x' | wc -l ";
	head = get_classified_tokens(line);
	if (!head)
		return (printf("head err"), 1);
	head = get_unqouted_tokens(head);
	if (!head)
		return (printf("quote err"), 1);
	temp = head;
	while (temp)
	{
		printf("** %s *\n", temp->value);
		temp = temp->next;
	}
	ft_tokenclear(&head);
	return (0);
}
