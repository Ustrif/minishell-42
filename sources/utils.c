/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:58:18 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/29 17:15:39 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quotes_closed(char *s)
{
	char	open;
	int		i;

	open = 0;
	i = 0;
	while (s[i])
	{
		if (!open && (s[i] == '\'' || s[i] == '"' ))
			open = s[i];
		else if (open && s[i] == open)
			open = 0;
		++i;
	}
	return (open != 0);
}

int	count_words(char **words)
{
	int	n;

	n = 0;
	if (!words)
		return (0);
	while (words[n])
		n++;
	return (n);
}

static char	**get_words(char **words, char *word)
{
	int		old_count;
	char	**new_words;
	int		i;

	old_count = count_words(words);
	new_words = malloc(sizeof(char *) * (old_count + 2));
	if (!new_words)
		return (NULL);
	i = 0;
	while (i < old_count)
	{
		new_words[i] = ft_strdup(words[i]);
		i++;
	}
	new_words[i] = ft_strdup(word);
	new_words[i + 1] = NULL;
	if (words)
		free_all(words);
	return (new_words);
}

char	**get_swords(char **words, char *word)
{
	char	**result;
	int		i;
	int		word_num;

	i = 0;
	word_num = count_words(words) + 1;
	result = get_words(words, word);
	if (!result)
		return (NULL);
	while (i < word_num)
	{
		if (!result[i])
		{
			free_all(result);
			return (NULL);
		}
		i++;
	}
	return (result);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*new;
	size_t	i;

	new = malloc(n + 1);
	i = 0;
	if (!new)
		return (NULL);
	while (i < n)
	{
		new[i] = s[i];
		i++;
	}
	new[n] = '\0';
	return (new);
}

t_env	*find_env(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

t_env	*create_env(const char *env_str)
{
	char	*equal;
	t_env	*node;
	size_t	key_len;

	equal = ft_strchr(env_str, '=');
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	if (equal)
	{
		key_len = equal - env_str;
		node->key = ft_strndup(env_str, key_len);
		node->value = ft_strdup(equal + 1);
	}
	else
	{
		node->key = ft_strdup(env_str);
		node->value = NULL;
	}
	node->next = NULL;
	return (node);
}

void	env_add_node(t_env **env_list, t_env *new_node)
{
	t_env	*tmp;

	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}
	tmp = *env_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}
