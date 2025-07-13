/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathcheck.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:41:00 by beinan            #+#    #+#             */
/*   Updated: 2025/07/13 11:50:48 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*fill1(const char *cmd, size_t cmd_len)
{
	char	*full;

	full = malloc(2 + cmd_len + 1);
	if (!full)
		return (NULL);
	ft_memcpy(full, "./", 2);
	ft_memcpy(full + 2, cmd, cmd_len + 1);
	return (full);
}

static char	*fill2(const char *cmd, size_t cmd_len,
		size_t dir_len, const char *p)
{
	char	*full;

	full = malloc(dir_len + 1 + cmd_len + 1);
	if (!full)
		return (NULL);
	ft_memcpy(full, p, dir_len);
	full[dir_len] = '/';
	ft_memcpy(full + dir_len + 1, cmd, cmd_len + 1);
	return (full);
}

char	*get_path(const char *cmd, const char *p)
{
	size_t		cmd_len;
	const char	*colon;
	size_t		dir_len;
	char		*full;

	cmd_len = ft_strlen(cmd);
	while (p && *p)
	{
		colon = ft_strchr(p, ':');
		dir_len = calculate_size(colon, p);
		if (dir_len == 0)
			full = fill1(cmd, cmd_len);
		else
			full = fill2(cmd, cmd_len, dir_len, p);
		if (!full)
			return (NULL);
		if (access(full, F_OK | X_OK) == 0)
			return (full);
		free(full);
		if (!colon)
			break ;
		p = colon + 1;
	}
	return (NULL);
}

char	*get_real_path(char *s, t_env *env)
{
	t_env	*e;
	char	*line;
	char	*result;

	if (!s || !env)
		return (NULL);
	if (s[0] == '/' || (s[0] == '.'
			&& (s[1] == '/' || (s[1] == '.' && s[2] == '/'))))
		return (ft_strdup(s));
	if (check_builtin(s))
		return (NULL);
	e = find_env(env, "PATH");
	if (!e)
		return (ft_strdup(s));
	line = e->value;
	if (ft_strchr(s, '/'))
		return (strdup(s));
	result = get_path(s, line);
	return (result);
}
