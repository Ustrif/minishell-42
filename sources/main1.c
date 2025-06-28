/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 12:53:10 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/26 16:05:12 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_cmd_args(char **cmd)
{
	if (!cmd)
	{
		printf("full_cmd: NULL\n");
		return;
	}

	printf("full_cmd:\n");
	int i;
	for (i = 0; cmd[i] != NULL; i++)
	{
		printf("  [%d]: %s\n", i, cmd[i]);
	}
	printf("  [%d]: %s\n", i, cmd[i]);
}

// Yapının tüm alanlarını yazdıran fonksiyon
void print_mini_struct(t_mini *mini)
{
	if (!mini)
	{
		printf("t_mini pointer is NULL\n");
		return;
	}

	print_cmd_args(mini->full_cmd);

	if (mini->full_path)
		printf("full_path: %s\n", mini->full_path);
	else
		printf("full_path: NULL\n");

	printf("infile: %d\n", mini->infile);
	printf("outfile: %d\n", mini->outfile);
}
/*
int	main(int argc, char **argv, char **env)
{
	t_token	*head;
	t_token	*temp;
	char	*line;

	if (!argc || !argv)
		return (1);
	//line = "e'c'ho \"hello      there\" how are 'you 'doing? $USER |wc -l >outfile";
	//line = "'echo' -n \"merhaba ssen\"|grep x";
	line = "cat << eof | grep 'x' | wc -l ";
	//line = "cat << eof";
	//line = "cat < a.txt | wc -l > num.txt";
	//line = "echo \"$USER\"'$USER'";
	head = get_classified_tokens(line);
	if (!head)
		return (printf("head err"), 1);
	head = get_unqouted_tokens(head);
	if (!head)
		return (printf("quote err"), 1);
	temp = head;
	t_list	*s;
	t_list	*del;
	s = get_full_minis(temp, env);
	del = s;
	while (s)
	{
		print_mini_struct(s->content);
		s = s->next;
	}
	ft_lstclear(&del, free_mini);
	ft_tokenclear(&head);
	return (0);
}*/
