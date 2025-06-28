/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 09:46:25 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/28 13:17:01 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_scode_data(char *s)
{
	
}

char	*get_expanded_data(const char *s)
{
	char	*res;

	res = get_expanded_data1(s, 0, 0, 0);
	if (!res)
		return (NULL);
	
	return (res);
}

t_token	*get_expanded_tokens(t_token *head)
{
	t_token	*temp;
	char	*val;

	if (!head)
		return (NULL);
	temp = head;
	while (temp)
	{
		val = temp->value;
		temp->value = get_expanded_data(val);
		free(val);
		if (!temp->value)
			ft_tokenclear(&head);
		temp = temp->next;
	}
	return (head);
}
