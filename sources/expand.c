/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:54:03 by codespace         #+#    #+#             */
/*   Updated: 2025/06/29 15:45:40 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_var_name(const char *s, int start, int *len)
{
	int		idx;

	idx = 0;
	while (s[start + idx]
		&& (ft_isalnum(s[start + idx]) || s[start + idx] == '_'))
		idx++;
	*len = idx;
	return (ft_substr(s, start, idx));
}

static char	*append_literal(const char *s, int start, int len, char *res)
{
	char	*part;
	char	*tmp;

	part = ft_substr(s, start, len);
	if (!part)
		return (NULL);
	tmp = ft_strjoin(res, part);
	free(part);
	free(res);
	return (tmp);
}

static char	*append_variable(const char *s, int *i, int *last, char *res)
{
	char	*var;
	char	*env;
	int		vlen;
	char	*tmp;

	if (!res)
		return (NULL);
	var = extract_var_name(s, *i + 1, &vlen);
	if (!var)
		return (NULL);
	env = getenv(var); // degisecek.
	free(var);
	if (!env)
		env = "";
	tmp = ft_strjoin(res, env);
	if (!tmp)
		return (NULL);
	free(res);
	res = tmp;
	*i += vlen + 1;
	*last = *i;
	return (res);
}

static int	update_i(int *i)
{
	*i = *i + 1;
	return (0);
}

char	*get_expanded_data1(const char *s, int i, int last, char open)
{
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (s[i])
	{
		if (!open && (s[i] == '\'' || s[i] == '"'))
			open = s[i++];
		else if (open && s[i] == open)
			open = update_i(&i);
		else if (s[i] == '$' && open != '\'' && s[i + 1]
			&& (ft_isalnum(s[i + 1]) || s[i + 1] == '_'))
		{
			result = append_literal(s, last, i - last, result);
			result = append_variable(s, &i, &last, result);
			if (!result)
				return (NULL);
		}
		else
			i++;
	}
	if (i > last)
		result = append_literal(s, last, i - last, result);
	return (result);
}
