/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:58:45 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/29 16:58:46 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sort_env_array(t_env **env_array, int count)
{
	int		i;
	int		j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (ft_strcmp(env_array[j]->key, env_array[j + 1]->key) > 0)
				swap_env_nodes(env_array, j, j + 1);
			j++;
		}
		i++;
	}
}

static t_env	**create_env_array(t_env *env_list, int count)
{
	t_env	**env_array;
	t_env	*tmp;
	int		i;

	env_array = malloc(sizeof(t_env *) * count);
	if (!env_array)
		return (NULL);
	tmp = env_list;
	i = 0;
	while (tmp)
	{
		env_array[i] = tmp;
		tmp = tmp->next;
		i++;
	}
	return (env_array);
}

static void	print_env_entry(t_env *env)
{
	write(1, "declare -x ", 11);
	write(1, env->key, ft_strlen(env->key));
	if (env->value)
	{
		write(1, "=\"", 2);
		write(1, env->value, ft_strlen(env->value));
		write(1, "\"", 1);
	}
	write(1, "\n", 1);
}

static void	display_env_list(t_env *env_list)
{
	t_env	**env_array;
	int		count;
	int		i;

	count = count_env_nodes(env_list);
	if (count == 0)
		return ;
	env_array = create_env_array(env_list, count);
	if (!env_array)
		return ;
	sort_env_array(env_array, count);
	i = 0;
	while (i < count)
	{
		print_env_entry(env_array[i]);
		i++;
	}
	free(env_array);
}

int	command_export(t_env **env_list, char **args)
{
	int	i;
	int	result;
	int	ret;

	if (!args[1])
	{
		display_env_list(*env_list);
		return (0);
	}
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			result = process_export_with_value(env_list, args[i]);
		else
			result = process_export_key_only(env_list, args[i]);
		if (result != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
/*
int process_export_key_only(t_env **env_list, const char *arg)
{
	if (!is_valid_env_key(arg))
    {
		write(2, "export: not a valid identifier\n", 32);
		return 1;
	}
	if (!find_env(*env_list, arg))
    {
		t_env *new_node = create_env(arg);
		env_add_node(env_list, new_node);
	}
	return (0);
}

int process_export_with_value(t_env **env_list, const char *arg)
{
	const char *equal = ft_strchr(arg, '=');
	char *key = ft_strndup(arg, equal - arg);
	char *value = ft_strdup(equal + 1);

	if (!is_valid_env_key(key))
    {
		write(2, "export: not a valid identifier\n", 32);
		return (free(key), free(value), 1);
	}
	t_env *found = find_env(*env_list, key);
	if (found)
    {
		free(found->value);
		found->value = value;
		free(key);
	}
    else
    {
		t_env *new_node = malloc(sizeof(t_env));
		new_node->key = key;
		new_node->value = value;
		new_node->next = NULL;
		env_add_node(env_list, new_node);
	}
	return (0);
}

static void	display_env_list(t_env *env_list)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		write(1, "declare -x ", 11);
		write(1, tmp->key, ft_strlen(tmp->key));
		if (tmp->value)
		{
			write(1, "=\"", 2);
			write(1, tmp->value, ft_strlen(tmp->value));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		tmp = tmp->next;
	}
}

int	command_export(t_env **env_list, char **args)
{
	int	i;
	int	ret;
	int	result;

	i = 1;
	ret = 0;
	if (!args[1])
	{
		display_env_list(*env_list);
		return (0);
	}
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			result = process_export_with_value(env_list, args[i]);
		else
			result = process_export_key_only(env_list, args[i]);
		if (result != 0)
			ret = 1;
		i++;
	}
	return (ret);
}


int command_export(t_env **env_list, char **args) {
	int i = 1;
	int ret = 0;

	if (!args[1]) {
		t_env *tmp = *env_list;
		while (tmp) {
			write(1, "declare -x ", 11);
			write(1, tmp->key, ft_strlen(tmp->key));
			if (tmp->value) {
				write(1, "=\"", 2); // ← was 3, fixed to 2 (" and =)
				write(1, tmp->value, ft_strlen(tmp->value));
				write(1, "\"", 1);
			}
			write(1, "\n", 1);
			tmp = tmp->next;
		}
		return 0;
	}

	while (args[i]) {
		int result;
		if (ft_strchr(args[i], '=')) {
			result = process_export_with_value(env_list, args[i]);
		} else {
			result = process_export_key_only(env_list, args[i]);
		}
		if (result != 0)
			ret = 1;
		i++;
	}
	return ret;
}
*/