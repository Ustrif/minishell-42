/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:03:44 by beinan            #+#    #+#             */
/*   Updated: 2024/11/05 14:46:27 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_fromthestart(char const *s1, char const *s2)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s1[i] && s2[j])
	{
		if (s1[i] == s2[j])
		{
			i++;
			j = 0;
			while (s1[i] != s2[j])
			{
				j++;
				if (s2[j] == '\0')
					return (i);
			}
		}
		else
			j++;
		if (s2[j] == '\0')
			return (i);
	}
	return (i);
}

static int	ft_fromtheend(const char *s1, char const *s2)
{
	int		i;
	int		j;

	i = 0;
	j = ft_strlen(s1) - 1;
	while (s2[i] && s1[j])
	{
		if (s1[j] == s2[i])
		{
			j--;
			i = 0;
			while (s1[j] != s2[i])
			{
				i++;
				if (s2[i] == '\0')
					return (j);
			}
		}
		else
			i++;
		if (s2[i] == '\0')
			return (j);
	}
	return (j);
}

char	*ft_strtrim(char const *s1, char const *s2)
{
	int		i;
	char	*str1;

	if (ft_strlen(s2) == 0)
		return (ft_strdup(s1));
	if (ft_strlen(s1) == 0)
	{
		str1 = malloc(1 * sizeof(char));
		if (str1 == NULL)
			return (NULL);
		str1[0] = '\0';
		return (str1);
	}
	i = ft_fromthestart(s1, s2);
	if (ft_fromthestart(s1, s2) > ft_fromtheend(s1, s2))
	{
		str1 = malloc(1 * sizeof(char));
		if (str1 == NULL)
			return (NULL);
		str1[0] = '\0';
		return (str1);
	}
	str1 = ft_substr(s1, i, ft_fromtheend(s1, s2) - i + 1);
	return (str1);
}
