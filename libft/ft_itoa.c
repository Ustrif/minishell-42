/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 16:23:20 by beinan            #+#    #+#             */
/*   Updated: 2024/11/05 14:41:15 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	new_len(long n, int len)
{
	long	m;

	m = n;
	if (m < 0)
	{
		m = -m;
		len++;
	}
	while (m >= 10)
	{
		m = m / 10;
		len++;
	}
	return (len);
}

static char	*all_positive(long m, char *num, int i, int len)
{
	int	len2;

	len2 = len;
	while (i < len)
	{
		num[len - 1] = m % 10 + 48;
		m = m / 10;
		len--;
	}
	num[len2] = '\0';
	return (num);
}

char	*ft_itoa(int n)
{
	char	*num;
	int		i;
	int		len;
	long	m;

	i = 0;
	m = n;
	len = new_len(m, 1);
	num = (char *)malloc(sizeof(char) * (len + 1));
	if (!num)
		return (NULL);
	else if (n < 0)
	{
		num[0] = '-';
		m = -m;
		i++;
	}
	num = all_positive(m, num, i, len);
	return (num);
}
