/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 15:08:30 by beinan            #+#    #+#             */
/*   Updated: 2024/10/22 13:01:40 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *string, int c)
{
	int		i;
	int		len;
	char	*str;

	str = (char *)string;
	i = 0;
	len = ft_strlen(str) + 1;
	while (i < len)
	{
		if (str[i] == (char)c)
			return (str + i);
		i++;
	}
	return (NULL);
}
