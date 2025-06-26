#include "minishell.h"

char *get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return env_list->value;
		env_list = env_list->next;
	}
	return NULL;
}