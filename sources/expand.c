/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:54:03 by codespace         #+#    #+#             */
/*   Updated: 2025/06/26 16:01:30 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
typedef struct s_env {
	char		*key;
	char		*value;
	struct s_env	*next;
} t_env;

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

t_env *init_env(char **envp)
{
	t_env *env_list = NULL;
	t_env *node;

	while (*envp)
	{
		node = create_env(*envp);
		env_add_node(&env_list, node);
		envp++;
	}
	return env_list;
}

char *get_env_value(t_env *env_list, char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return env_list->value;
		env_list = env_list->next;
	}
	return NULL;
}

void	update_open(char *s, char *open, int i)
{
	if (!(*open) && s[i] == '\'')
		(*open) = '\'';
	else if ((*open) == '\'' && s[i] == '\'')
		(*open) = 0;
}

char	*get_expanded_data1(char *s, int i, int last_i, char open, t_env	*env_list)
{
	int		y;
	char	*result = "";
	char	*value;
	char	*temp;
	char	*prefix;

	while (s[i])
	{
		update_open(s, &open, i);
		if (s[i] == '$' && !open && s[i+1] &&
			(ft_isalpha(s[i+1]) || s[i+1] == '_'))
		{
			prefix = ft_substr(s, last_i, i - last_i);
			y = 1;
			while (s[i+y] && (ft_isalnum(s[i+y]) || s[i+y] == '_'))
				y++;
			{
				char *varname = ft_substr(s, i+1, y-1);
				//printf("%s\n", varname);
				if (!varname)
				{
					free(prefix);
					return (NULL);
				}
				//value = ft_strdup("STRDUP!"); //expand(varname); -> expand(NULL) sorun olmamalı.
				
				value = get_env_value(env_list, varname);
				free(varname);

				if (!value)
				{
					free(prefix);
					return (NULL);
				}
			}
			
			//printf("%s\n",prefix);
			temp = ft_strjoin(result, prefix);
			//printf("%s",temp);
			free(prefix);
			
			if (!temp)
			{
				free(value);
				return NULL;
			}
			if (value)
				result = ft_strjoin(temp, value);
			else 
				result = ft_strdup(temp);
			free(temp);
			free(value);
			if (!result)
				return NULL;
			i += y - 1;
			last_i = i + 1;
		}
		i++;
	}
	temp = ft_substr(s, last_i, i - last_i);
	if (!temp)
		return (free(result), NULL);
	prefix = ft_strjoin(result, temp);
	free(temp);
	free(result);
	return (prefix);
}

char *get_expanded_data(char *s, t_env	*env_list)
{
	char	*res;
	//printf("%s", env_list->value);
	res = get_expanded_data1(s, 0, 0, 0, env_list);
	return (res);
}

int	main(int ac, char **av, char **envp)
{
	char	*res;
	(void)ac;
	(void)av;

	t_env	*env_list = init_env(envp);
	char *test = "USER";
	if(!get_env_value(env_list, test))
		printf("yokkkk\n");
	else 
		printf("%s\n", get_env_value(env_list, test));

	//printf("%s", env_list->key);
	res = get_expanded_data("\"$USERdeneme.-----$USER-$?\"", env_list);
	//res = get_expanded_data("\" $USER \"", env_list);
	printf("%s \n", res);
	return(0);
}
*/