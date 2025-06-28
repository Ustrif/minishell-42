/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:46:15 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/28 19:46:46 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

char	*get_scode_data(const char *s)
{
	char	*num;
	char	*result;

	num = ft_itoa(g_status);
	if (!num)
		return (NULL);
	result = get_scode_data1(s, 0, 0, num);
	free(num);
	return (result);
}

char	*get_expanded_data(const char *s)
{
	char	*res;
	char	*dolarmark;

	res = get_expanded_data1(s, 0, 0, 0);
	if (!res)
		return (NULL);
	dolarmark = get_scode_data(res);
	free(res);
	return (dolarmark);
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
