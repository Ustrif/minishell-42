/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:45:34 by beinan            #+#    #+#             */
/*   Updated: 2024/11/05 18:59:08 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	char				*loc;
	char				*str;
	unsigned int		i;

	str = (char *)s;
	i = 0;
	loc = NULL;
	while (n > 0)
	{
		if (str[i] == (char)c)
		{
			loc = &str[i];
			return (loc);
		}
		i++;
		n--;
	}
	return (NULL);
}
