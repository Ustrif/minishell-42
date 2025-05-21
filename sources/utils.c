/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:59:53 by raydogmu          #+#    #+#             */
/*   Updated: 2025/04/30 21:07:21 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quotes_closed(char *s)
{
	char	open;
	int		i;

	open = 0;
	i = 0;
	while (s[i])
	{
		if (!open && (s[i] == '\'' || s[i] == '"' ))
			open = s[i];
		else if (open && s[i] == open)
			open = 0;
		++i;
	}
	return (open != 0);
}
