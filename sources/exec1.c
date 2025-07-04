/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:17:59 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/04 20:56:30 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	execute1(t_mini *cmd)
{
	struct stat	sb;

	if (stat(cmd->full_path, &sb) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->full_path, 2);
		perror("");
		return (127);
	}
	if (S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->full_path, 2);
		ft_putendl_fd(": Is a directory", 2);
		return (126);
	}
	return (0);
}

void	execute_cmd(t_mini *cmd, char **envp)
{
	int	err;

	if (!cmd->full_path && !cmd->full_cmd)
		exit (0);
	if (!cmd->full_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->full_cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	err = execute1(cmd);
	if (err > 0)
		exit(err);
	if (access(cmd->full_path, X_OK) != 0)
	{
		perror("minishell: permission denied");
		exit(126);
	}
	execve(cmd->full_path, cmd->full_cmd, envp);
	perror("minishell: execve");
	exit(1);
}

void	sig_hand(int signum)
{
	if (signum)
		g_status = 130;
}
