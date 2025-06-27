/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:47:17 by codespace         #+#    #+#             */
/*   Updated: 2025/06/26 16:02:45 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*get_full_minis(t_token *head, char **env)
{
	t_list	*result;
	t_list	*temp;
	t_mini	*mini;

	result = get_minis(head);
	if (!result)
		return (printf("err"), NULL);
	temp = result;
	while (temp)
	{
		mini = temp->content;
		mini->full_path = get_path(mini->full_cmd[0], env);
		temp->content = mini;
		temp = temp->next;
	}
	return (result);
}
