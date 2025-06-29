/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:59:14 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/29 16:59:15 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_env_key(const char *key)
{
	int	i;

	i = 0;
	if (!key || !(key[0] == '_' || ft_isalpha(key[0])))
		return (0);
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	env_remove_node(t_env **env_list, const char *key)
{
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env_list;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env_list = curr->next;
			free(curr->key);
			if (curr->value)
				free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	command_unset(char **args, t_env **env_list)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_env_key(args[i]))
		{
			write(2, "unset: not a valid identifier: ", 32);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "\n", 1);
		}
		else
			env_remove_node(env_list, args[i]);
		i++;
	}
	return (0);
}
