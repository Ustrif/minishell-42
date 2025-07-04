/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:09:00 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/04 18:10:07 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*node;

	env_list = NULL;
	while (*envp)
	{
		node = create_env(*envp);
		env_add_node(&env_list, node);
		envp++;
	}
	return (env_list);
}
