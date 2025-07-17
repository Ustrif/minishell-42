/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 03:04:26 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/16 17:44:52 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	fill_cmd_array(t_list *cmds, t_mini **arr, int count)
{
	int	i;

	i = 0;
	while (i < count && cmds)
	{
		arr[i++] = cmds->content;
		cmds = cmds->next;
	}
}

void	init_pipes(int count, int ***pipes, pid_t **pids, t_mini ***arr)
{
	int	i;
	int	j;

	i = 0;
	while (i < count - 1)
	{
		(*pipes)[i] = malloc(sizeof(int) * 2);
		if (!(*pipes)[i] || pipe((*pipes)[i]) < 0)
		{
			perror("pipe");
			j = 0;
			while (j <= i)
			{
				free((*pipes)[j]);
				j++;
			}
			free(*arr);
			free(*pids);
			free(*pipes);
			exit(1);
		}
		i++;
	}
}

void	close_and_free_pipes(int **pipes, int count)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < count - 1)
	{
		if (pipes[i])
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			free(pipes[i]);
		}
		i++;
	}
	free(pipes);
}

void	wait_for_children(pid_t *pids, int count, int *err_code)
{
	int	i;
	int	wstatus;

	i = 0;
	while (i < count)
	{
		if (waitpid(pids[i], &wstatus, 0) < 0)
			continue ;
		if (i == count - 1)
		{
			if (WIFEXITED(wstatus))
				*err_code = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
			{
				*err_code = 128 + WTERMSIG(wstatus);
				if (*err_code == 139)
					write(2, "Segmentation fault (core dumped)\n", 34);
			}
		}
		i++;
	}
}

int	execute_pipeline(t_promp *promp)
{
	int		count;
	t_mini	**arr;
	pid_t	*pids;
	int		**pipes;

	count = ft_lstsize(promp->cmds);
	if (count == 0)
		return (0);
	if (allocate(count, &arr, &pids, &pipes) < 0)
		return (0);
	fill_cmd_array(promp->cmds, arr, count);
	preprocess_heredocs(arr, count);
	if (g_status == 130)
		return (heredoc_exit(arr, pids, pipes));
	if (count == 1 && is_builtin(arr[0]->full_cmd))
		return (one_command(promp, arr, pids, pipes));
	init_pipes(count, &pipes, &pids, &arr);
	spawn_children(arr, pipes, pids, promp);
	close_and_free_pipes(pipes, count);
	wait_for_children(pids, count, promp->err_code);
	free(arr);
	free(pids);
	return (0);
}
