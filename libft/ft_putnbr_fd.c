/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:51:41 by beinan            #+#    #+#             */
/*   Updated: 2024/11/05 18:45:55 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	make_it_positive(int nb, int fd)
{
	write (fd, "-", 1);
	nb = -1 * nb;
	return (nb);
}

static void	define_l(int nb, int fd)
{
	int	l;

	l = nb % 10;
	l = l + 48;
	write(fd, &l, 1);
}

void	ft_putnbr_fd(int nb, int fd)
{
	int	a;
	int	s;

	if (nb == -2147483648)
	{
		write(fd, "-2147483648", 11);
	}
	else if (nb < 0)
	{
		nb = make_it_positive(nb, fd);
		ft_putnbr_fd(nb, fd);
	}
	else if (nb <= 9)
	{
		s = nb + 48;
		write(fd, &s, 1);
	}
	else if (nb > 9)
	{
		a = nb / 10;
		ft_putnbr_fd(a, fd);
		define_l(nb, fd);
	}
}
