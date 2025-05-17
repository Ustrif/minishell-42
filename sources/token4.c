/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:58:43 by beinan            #+#    #+#             */
/*   Updated: 2025/04/19 14:58:43 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_tokens(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (head)
	{
		if (!head->value)
		{
			ft_tokenclear(&temp);
			return (1);
		}
		head = head->next;
	}
	return (0);
}

t_token	*get_tokens(const char *line)
{
	t_token	*head;
	char	*trimmed;

	trimmed = ft_strtrim(line, " ");
	if (trimmed == NULL)
		return (NULL);
	head = split_blanks(trimmed, 0, 0, 0);
	free(trimmed);
	if (head == NULL)
		return (NULL);
	refine_tokens(&head, split_pipes1);
	refine_tokens(&head, split_redirs1);
	if (check_tokens(head))
		return (NULL);
	return (head);
}
