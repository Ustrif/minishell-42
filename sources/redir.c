/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 04:19:40 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/16 16:38:25 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*ft_redirnnew(char *v, t_redir_type type)
{
	t_redir	*result;

	if (!v)
		return (NULL);
	result = malloc(sizeof(t_redir));
	if (result == NULL)
	{
		free(v);
		return (NULL);
	}
	result->target = v;
	result->type = type;
	result->heredoc_fd = 0;
	result->next = NULL;
	return (result);
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
