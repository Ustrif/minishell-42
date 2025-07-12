/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:10:12 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/12 11:37:50 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*get_full_minis(t_token *head, t_env *tenv)
{
	t_list	*result;
	t_list	*temp;
	t_mini	*mini;

	if (check_syntax(head))
		return (NULL);
	result = get_minis(head);
	if (!result)
		return (NULL);
	temp = result;
	while (temp)
	{
		mini = temp->content;
		if (mini->full_cmd)
			mini->full_path = get_real_path(mini->full_cmd[0], tenv);
		temp->content = mini;
		temp = temp->next;
	}
	return (result);
}
