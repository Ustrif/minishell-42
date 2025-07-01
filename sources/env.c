/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 19:02:36 by codespace         #+#    #+#             */
/*   Updated: 2025/07/01 19:04:09 by raydogmu         ###   ########.fr       */
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
