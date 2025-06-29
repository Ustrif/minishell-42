/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:58:35 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/29 16:58:36 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_export_with_value(t_env **env_list, const char *arg)
{
	const char	*equal;
	char		*key;
	char		*value;
	t_env		*found;

	equal = ft_strchr(arg, '=');
	key = ft_strndup(arg, equal - arg);
	value = ft_strdup(equal + 1);
	if (!is_valid_env_key(key))
		return (handle_export_error(key, value));
	found = find_env(*env_list, key);
	if (found)
		return (update_existing_env(found, key, value));
	else
		return (create_new_env_node(env_list, key, value));
}

int	count_env_nodes(t_env *env_list)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env_list;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

void	swap_env_nodes(t_env **env_array, int i, int j)
{
	t_env	*temp;

	temp = env_array[i];
	env_array[i] = env_array[j];
	env_array[j] = temp;
}
