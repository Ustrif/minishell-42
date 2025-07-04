/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 19:25:14 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/04 19:38:23 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_pipes(int **pipes, int count)
{
	int	j;

	j = 0;
	while (j < count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

void	run_child_command(t_mini *cmd, t_promp *promp)
{
	int	ret;

	if (setup_redirections(cmd))
		exit(1);
	if (is_builtin(cmd->full_cmd))
	{
		ret = exec_builtin(cmd, &promp->tenv);
		exit(ret);
	}
	execute_cmd(cmd, promp->envp);
	exit(1);
}

void	spawn_children(t_mini **arr, int **pipes, pid_t *pids, t_promp *promp)
{
	int		count;
	pid_t	pid;
	int		i;

	count = ft_lstsize(promp->cmds);
	i = 0;
	while (i < count)
	{
		pid = fork();
		if (pid < 0)
			exit(1);
		signal(SIGINT, sig_hand);
		if (pid == 0)
		{
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);
			if (i < count - 1)
				dup2(pipes[i][1], STDOUT_FILENO);
			close_pipes(pipes, count);
			run_child_command(arr[i], promp);
		}
		pids[i] = pid;
		i++;
	}
}
