/* ************************************************************************************** */
/*                                                                                        */
/*                                                                   :::      ::::::::    */
/*   file.c                                                        :+:      :+:    :+:    */
/*                                                               +:+ +:+         +:+      */
/*   By: raydogmu <raydogmu@student.42istanbul.com.tr>         +#+  +:+       +#+         */
/*                                                           +#+#+#+#+#+   +#+            */
/*   Created: 2025/05/22 23:00:23 by raydogmu                     #+#    #+#              */
/*   Updated: 2025/05/22 23:00:23 by raydogmu                    ###   ########.tr        */
/*                                                                                        */
/* ************************************************************************************** */

#include "minishell.h"

int	open_file(char *filename, int in_out)
{
	int	x;

	x = 0 ;
	if (in_out == 0)
		x = open(filename, O_RDONLY);
	if (x < 0)
	{
		perror("Error");
		exit(1);
	}
	if (in_out == 1)
		x = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (x < 0)
	{
		perror("Error");
		exit(1);
	}
	return (x);
}
