/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   parser.c                                                      :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: raydogmu <raydogmu@student.42istanbul.com.tr>         +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/05/22 23:00:43 by raydogmu                     #+#    #+#              */
/*   Updated: 2025/05/22 23:00:43 by raydogmu                    ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"

t_mini	*init_mini(void)
{
	t_mini	*new;

	new = malloc(sizeof(t_mini));
	if (!new)
		return (NULL);
	new->full_cmd = NULL;
	new->full_path = NULL;
	new->infile = STDIN_FILENO;
	new->outfile = STDOUT_FILENO;
	return (new);
}
