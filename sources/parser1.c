/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:01:10 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/16 11:20:53 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_redir(t_redir **list, t_redir_type type, const char *target)
{
	t_redir	*new;
	t_redir	*cur;

	new = ft_redirnnew(ft_strdup(target), type);
	if (!new)
		return (0);
	if (*list == NULL)
		*list = new;
	else
	{
		cur = *list;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
	return (1);
}

void	*cleanup(t_mini *mini, char **full_cmd, t_list **cmds)
{
	t_redir	*r;
	t_redir	*tmp;

	if (full_cmd)
		free_all(full_cmd);
	if (mini)
	{
		r = mini->redir;
		while (r)
		{
			tmp = r->next;
			free(r->target);
			free(r);
			r = tmp;
		}
		free(mini);
	}
	if (cmds)
		ft_lstclear(cmds, free_mini);
	return (NULL);
}
