/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basics.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 10:55:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/29 10:59:21 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	print_error(char *s, int i)
{
	g_status = 1;
	printf("minishell: %s\n", s);
	return (i);
}

int	basics(char *s)
{
	if (is_quotes_closed(s))
		return (print_error("cannot interpret unclosed quotes", 1));
	return (0);
}
