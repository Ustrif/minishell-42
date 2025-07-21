/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 10:23:59 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/21 10:09:47 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_code(int code, t_env *env)
{
	del_env_list(env);
	write(1, "exit\n", 5);
	if (code < -256)
		return (code + 600);
	else
		return (-code);
}

int	heredoc_exit(t_mini **arr, pid_t *pids, int **pipes)
{
	free(arr);
	free(pids);
	free(pipes);
	return (0);
}
