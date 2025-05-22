/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   utils.c                                                       :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: raydogmu <raydogmu@student.42istanbul.com.tr>         +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/05/22 23:01:32 by raydogmu                     #+#    #+#              */
/*   Updated: 2025/05/22 23:01:32 by raydogmu                    ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

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
