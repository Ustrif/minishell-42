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

t_token	*ft_tokennew(char *v)
{
	t_token	*result;

	result = malloc(sizeof(t_token));
	if (result == NULL)
		return (NULL);
	result->value = v;
	result->type = T_WORD;
	result->next = NULL;
	return (result);
}

void	ft_tokenadd_back(t_token **lst, t_token *new)
{
	t_token	*temp;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

void	ft_tokenclear(t_token **lst)
{
	struct s_token	*temp;

	while (*lst)
	{
		free((*lst)->value);
		temp = (*lst)->next;
		free(*lst);
		*lst = temp;
	}
}

int	ft_tokensize(t_token *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

void	ft_tokenadd_front(t_token **lst, t_token *new)
{
	new->next = *lst;
	*lst = new;
}
