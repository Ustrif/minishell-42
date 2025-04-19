/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:57:57 by beinan            #+#    #+#             */
/*   Updated: 2025/04/19 14:58:01 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static char	*adress(const char *str, int k, int z)
{
	char	*result;
	int		i;

	i = 0;
	result = malloc(z - k + 1);
	if (result == NULL)
		return (NULL);
	while (k < z)
	{
		result[i] = str[k];
		i++;
		k++;
	}
	result[i] = '\0';
	return (result);
}

static int	word_num(char const *s, char c)
{
	int		i;
	int		last_i;
	int		result;

	i = 0;
	last_i = 0;
	result = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			if (last_i < i)
				result++;
			last_i = i + 1;
		}
		else if (s[i + 1] == '\0')
			result++;
		i++;
	}
	return (result);
}

static char	**fake_split(char const *s, char c)
{
	int		i;
	int		j;
	int		last_i;
	char	**result;

	i = 0;
	last_i = 0;
	j = 0;
	result = malloc((word_num(s, c) + 1) * sizeof(char *));
	if (result == NULL)
		return (NULL);
	while (s[i])
	{
		if (s[i] == c)
		{
			if (last_i < i)
				result[j++] = adress(s, last_i, i);
			last_i = i + 1;
		}
		else if (s[i + 1] == '\0')
			result[j++] = adress(s, last_i, i + 1);
		i++;
	}
	result[j] = NULL;
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**text;
	int		i;
	int		word_s;

	text = fake_split(s, c);
	if (!s || text == NULL)
		return (NULL);
	i = 0;
	word_s = word_num(s, c);
	while (i < word_s)
	{
		if (text[i] == NULL)
		{
			i = 0;
			while (i < word_s)
			{
				free(text[i]);
				i++;
			}
			free(text);
			return (NULL);
		}
		i++;
	}
	return (text);
}
