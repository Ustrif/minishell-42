/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basics.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 10:55:32 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/07 13:43:42 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_error(char *s, int i)
{
	printf("minishell: %s\n", s);
	return (i);
}

int	is_blank(char *s)
{
	if (s && ft_strlen(s) == 0)
		return (1);
	return (0);
}

int	only_quote(char *s)
{
	if (!s)
		return (1);
	if (ft_strlen(s) == 2)
	{
		if (s[0] == '\'' && s[1] == '\'')
			return (1);
		if (s[0] == '"' && s[1] == '"')
			return (1);
	}
	return (0);
}

int	basics(char *s, int *err_code)
{
	if (is_quotes_closed(s))
	{
		*err_code = 1;
		return (print_error("cannot interpret unclosed quotes", 1));
	}
	if (is_blank(s))
	{
		*err_code = 0;
		return (1);
	}
	if (only_quote(s))
	{
		*err_code = 127;
		return (print_error(": command not found", 1));
	}
	return (0);
}
