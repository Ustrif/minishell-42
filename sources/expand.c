/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 11:54:03 by codespace         #+#    #+#             */
/*   Updated: 2025/06/26 13:46:48 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include "minishell.h"

void	update_open(char *s, char *open, int i)
{
	if (!(*open) && s[i] == '\'')
		(*open) = '\'';
	else if ((*open) == '\'' && s[i] == '\'')
		(*open) = 0;
}

char	*get_expanded_data1(char *s, int i, int last_i, char open)
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
				if (!varname)
				{
					free(prefix);
					return (NULL);
				}
				value = ft_strdup("bu"); //expand(varname); -> expand(NULL) sorun olmamalı.
				free(varname);
				if (!value)
				{
					free(prefix);
					return (NULL);
				}
			}
			temp = ft_strjoin(result, prefix);
			free(prefix);
			if (!temp)
			{
				free(value);
				return NULL;
			}
			result = ft_strjoin(temp, value);
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

char *get_expanded_data(char *s)
{
	char	*res;

	res = get_expanded_data1(s, 0, 0, 0);
	return (res);
}

int	main(void)
{
	char	*res;

	res = get_expanded_data("echo $USERdeneme.'$USER' $?");
	printf("%s \n", res);
	return(0);
}
*/