#include "minishell.h"

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

/*
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