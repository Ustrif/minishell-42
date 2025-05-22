/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:59:53 by raydogmu          #+#    #+#             */
/*   Updated: 2025/04/30 21:07:21 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_tokendel(t_token *token)
{
	if (token == NULL)
		return ;
	free(token->value);
	free(token);
}

t_token	*ft_tokenlast(t_token *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	refine_tokens(t_token **head, t_token *(*split_func)(char *))
{
	t_token	*cur;
	t_token	*prev;
	t_token	*next;
	t_token	*split;

	cur = *head;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		split = split_func(cur->value);
		if (split)
		{
			if (prev)
				prev->next = split;
			else
				*head = split;
			prev = ft_tokenlast(split);
			prev->next = next;
			ft_tokendel(cur);
		}
		else
			prev = cur;
		cur = next;
	}
}
