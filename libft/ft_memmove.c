/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:35:51 by beinan            #+#    #+#             */
/*   Updated: 2024/11/05 14:41:30 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned int	i;
	char			*src1;
	char			*dest;

	i = len;
	src1 = (char *)src;
	dest = (char *)dst;
	if (dst == src)
		return (dst);
	else if (dest > src1)
	{
		while (i-- > 0)
			*(dest + i) = *(src1 + i);
	}
	else
	{
		i = 0;
		while (i < len)
		{
			*(dest + i) = *(src1 + i);
			i++;
		}
	}
	return (dest);
}
