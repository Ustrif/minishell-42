/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_re2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 08:30:39 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/01 17:22:55 by codespace        ###   ########.fr       */
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
