/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 04:19:40 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/12 19:11:35 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*ft_redirnnew(char *v, t_redir_type type)
{
	t_redir	*result;

	result = malloc(sizeof(t_redir));
	if (result == NULL)
		return (NULL);
	result->target = v;
	result->type = type;
	result->heredoc_fd = 0;
	result->next = NULL;
	return (result);
}

void	ft_rediradd_back(t_redir **lst, t_redir *new)
{
	t_redir	*temp;

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

void	ft_redirclear(t_redir **lst)
{
	struct s_redir	*temp;

	while (*lst)
	{
		free((*lst)->target);
		if ((*lst)->heredoc_fd > 0)
			close((*lst)->heredoc_fd);
		temp = (*lst)->next;
		free(*lst);
		*lst = temp;
	}
}
