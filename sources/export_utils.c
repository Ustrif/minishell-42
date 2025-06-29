#include "minishell.h"

int	process_export_key_only(t_env **env_list, const char *arg)
{
	t_env	*new_node;

	if (!is_valid_env_key(arg))
	{
		write(2, "bash: export: `", 16);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		return (1);
	}
	if (!find_env(*env_list, arg))
	{
		new_node = create_env(arg);
		if (!new_node)
			return (1);
		env_add_node(env_list, new_node);
	}
	return (0);
}

int	handle_export_error(char *key, char *value)
{
	write(2, "bash: export: `", 16);
	write(2, key, ft_strlen(key));
	write(2, "': not a valid identifier\n", 26);
	free(key);
	free(value);
	return (1);
}

int	update_existing_env(t_env *found, char *key, char *value)
{
	free(found->value);
	found->value = value;
	free(key);
	return (0);
}

int	create_new_env_node(t_env **env_list, char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(key);
		free(value);
		return (1);
	}
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	env_add_node(env_list, new_node);
	return (0);
}
