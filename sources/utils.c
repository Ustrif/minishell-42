#include "minishell.h"

char *ft_strndup(const char *s, size_t n) 
{
    char *new = malloc(n + 1);
    size_t i = 0;

    if (!new)
        return NULL;
    while (i < n)
    {
        new[i] = s[i];
        i++;
    }
    new[n] = '\0';
    return new;
}

t_env *find_env(t_env *env, const char *key)
{
	while (env)
    {
		if (ft_strcmp(env->key, key) == 0)
			return env;
		env = env->next;
	}
	return NULL;
}

t_env *create_env(const char *env_str)
{
	char *equal = ft_strchr(env_str, '=');
	t_env *node = malloc(sizeof(t_env));

	if (!node) 
        return NULL;
	if (equal)
    {
		size_t key_len = equal - env_str;
		node->key = ft_strndup(env_str, key_len);
		node->value = ft_strdup(equal + 1);
	} 
    else
    {
		node->key = ft_strdup(env_str);
		node->value = NULL;
	}
	node->next = NULL;
	return node;
}

void env_add_node(t_env **env_list, t_env *new_node)
{
	if (!*env_list)
    {
		*env_list = new_node;
		return ;
	}
	t_env *tmp = *env_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}
