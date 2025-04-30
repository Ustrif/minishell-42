/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:41:08 by beinan            #+#    #+#             */
/*   Updated: 2025/04/30 17:41:14 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
