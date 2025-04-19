/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 13:30:50 by beinan            #+#    #+#             */
/*   Updated: 2024/11/05 14:43:33 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *des, const char *src, size_t size)
{
	unsigned int	i;
	unsigned int	len_s;
	unsigned int	len_d;
	char			*str;

	str = (char *)src;
	i = 0;
	len_s = 0;
	len_d = 0;
	while (str[len_s])
		len_s++;
	while (des[len_d])
		len_d++;
	if (size > len_d)
	{
		while (i + 1 < size - len_d && str[i])
		{
			des[len_d + i] = str[i];
			i++;
		}
		des[len_d + i] = '\0';
		return (len_d + len_s);
	}
	return (len_s + size);
}
