/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:46:15 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/01 19:45:18 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_scode_data(const char *s, int err_code)
{
	char	*num;
	char	*result;

	num = ft_itoa(err_code);
	if (!num)
		return (NULL);
	result = get_scode_data1(s, 0, 0, num);
	free(num);
	return (result);
}

char	*get_expanded_data(const char *s, int err_code)
{
	char	*res;
	char	*dolarmark;

	res = get_expanded_data1(s, 0, 0, 0);
	if (!res)
		return (NULL);
	dolarmark = get_scode_data(res, err_code);
	free(res);
	return (dolarmark);
}
