/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_re2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:30:39 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/04 18:10:30 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_real_path(char *s, char **env)
{
	static char	**e = NULL;

	if (e == NULL)
	{
		e = env;
		return (NULL);
	}
	if (!s)
		return (NULL);
	else if (s[0] == '/' || (s[0] == '.'
			&& (s[1] == '/' || (s[1] == '.' && s[2] == '/'))))
		return (ft_strdup(s));
	return (get_path(s, e));
}
