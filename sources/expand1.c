/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 09:46:25 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/30 09:37:20 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	status_num(const char *s)
{
	int		i;
	int		count;
	char	open;

	open = 0;
	i = 0;
	count = 0;
	while (s[i])
	{
		if (!open && (s[i] == '"' || s[i] == '\''))
			open = s[i++];
		else if (open && s[i++] == open)
			open = 0;
		else if (s[i] == '$' && open != '\'' && s[i + 1] == '?')
		{
			count++;
			i += 2;
		}
		else
			i++;
	}
	return (count);
}

void	update_result(char *num, char *result, int *j)
{
	int	k;
	int	num_len;

	k = 0;
	num_len = ft_strlen(num);
	while (k < num_len)
	{
		result[(*j)] = num[k];
		(*j)++;
		k++;
	}
}

int	close_quote(const char *s, int *i, int *j, char *res)
{
	res[(*j)++] = s[(*i)++];
	return (0);
}

char	*for_result(char *res, int j)
{
	res[j] = '\0';
	return (res);
}

char	*get_scode_data1(const char *s, int i, char open, char *num)
{
	int		j;
	char	*result;

	result = malloc(ft_strlen(s) + status_num(s) + 1);
	if (!result)
		return (NULL);
	j = 0;
	while (s[i])
	{
		if (!open && (s[i] == '"' || s[i] == '\''))
		{
			open = s[i++];
			result[j++] = open;
		}
		else if (open && s[i] == open)
			open = close_quote(s, &i, &j, result);
		else if (s[i] == '$' && open != '\'' && s[i + 1] == '?')
		{
			update_result(num, result, &j);
			i += 2;
		}
		else
			result[j++] = s[i++];
	}
	return (for_result(result, j));
}
