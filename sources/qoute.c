/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qoute.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:20:18 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/20 09:39:54 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*remove_quotes(char *s)
{
	int		i;
	int		j;
	char	quote;
	char	*result;

	i = 0;
	j = 0;
	quote = 0;
	if (!s)
		return (NULL);
	result = malloc(ft_strlen(s) + 1);
	if (!result)
		return (NULL);
	while (s[i])
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i++];
		else if (quote && s[i] == quote)
			quote = 0, i++;
		else
			result[j++] = s[i++];
	}
	result[j] = '\0';
	return (result);
}

t_token	*get_unqouted_tokens(t_token *head)
{
	t_token	*temp;
	char	*value;

	temp = head;
	while (temp)
	{
		value = remove_quotes(temp->value);
		free(temp->value);
		temp->value = value;
		temp = temp->next;
	}
	temp = head;
	while (temp)
	{
		if (!temp->value)
			return (ft_tokenclear(&head), NULL);
		temp = temp->next;
	}
	return (head);
}
