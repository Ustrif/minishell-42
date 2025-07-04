/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:01:15 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/04 19:45:42 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	allocate(int count, t_mini ***arr, pid_t **pids, int ***pipes)
{
	*arr = malloc(sizeof(t_mini *) * count);
	*pids = malloc(sizeof(pid_t) * count);
	*pipes = malloc(sizeof(int *) * (count - 1));
	if (!*arr || !*pids || (count > 1 && !*pipes))
	{
		free(arr);
		free(pids);
		free(pipes);
		return (-1);
	}
	return (0);
}

void	free_three(t_mini **arr, pid_t *pids, int **pipes)
{
	free(arr);
	free(pids);
	free(pipes);
}

void	do_files(int sin, int sout)
{
	dup2(sin, STDIN_FILENO);
	dup2(sout, STDOUT_FILENO);
	close(sin);
	close(sout);
}

void	one_command(t_promp	*promp, t_mini **arr, pid_t *pids, int **pipes)
{
	int	saved_stdin;
	int	saved_stdout;
	int	redir_status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	redir_status = setup_redirections(arr[0]);
	if (redir_status == -130)
	{
		g_status = 130;
		do_files(saved_stdin, saved_stdout);
		free_three(arr, pids, pipes);
		return ;
	}
	if (redir_status)
	{
		*(promp->err_code) = 1;
		do_files(saved_stdin, saved_stdout);
		free_three(arr, pids, pipes);
		return ;
	}
	*(promp->err_code) = exec_builtin(arr[0], &promp->tenv);
	do_files(saved_stdin, saved_stdout);
	free_three(arr, pids, pipes);
	return ;
}
