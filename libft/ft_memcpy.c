/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:49:16 by beinan            #+#    #+#             */
/*   Updated: 2024/11/05 14:41:27 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*ddest;
	unsigned char	*ssrc;
	unsigned int	i;

	if (!dest && !src)
		return (dest);
	i = 0;
	ddest = dest;
	ssrc = (unsigned char *)src;
	while (i < n)
	{
		ddest[i] = ssrc[i];
		i++;
	}
	return (dest);
}
