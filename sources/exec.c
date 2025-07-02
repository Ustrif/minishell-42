/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 03:04:26 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/02 11:26:49 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void execute_cmd(t_mini *cmd, char **envp)
{
	struct stat sb;

	if (!cmd->full_path && !cmd->full_cmd)
		exit (0);
	if (!cmd->full_path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd->full_cmd[0]);
		exit(127);
	}
	if (stat(cmd->full_path, &sb) == -1)
	{
		perror("minishell: stat");
		exit(127); // ENOENT veya benzeri durumlar için
	}
	if (S_ISDIR(sb.st_mode))
	{
		fprintf(stderr, "minishell: %s: Is a directory\n", cmd->full_path);
		exit(126); // Bash bu durumda 126 döner
	}
	if (access(cmd->full_path, X_OK) != 0)
	{
		perror("minishell: permission denied");
		exit(126); // Bash bu durumda da 126 döner
	}
	execve(cmd->full_path, cmd->full_cmd, envp);
	perror("minishell: execve");
	exit(1);
}

int setup_redirections(t_mini *cmd)
{
    t_redir *r = cmd->redir;
    int      fd;

    while (r)
    {
        if (r->type == R_IN)
            fd = open(r->target, O_RDONLY);
        else if (r->type == R_HEREDOC)
            fd = open_heredoc(r->target);
        else if (r->type == R_OUT)
            fd = open(r->target, O_CREAT|O_TRUNC|O_WRONLY, 0644);
        else
            fd = open(r->target, O_CREAT|O_APPEND|O_WRONLY, 0644);

        if (fd < 0)
        {
            perror(r->target);
            return (1);
        }

        if (r->type == R_IN || r->type == R_HEREDOC)
            dup2(fd, STDIN_FILENO);
        else
            dup2(fd, STDOUT_FILENO);

        close(fd);
        r = r->next;
    }
    return (0); // success
}

void execute_pipeline(t_promp *promp)
{
    int     count = 0;
    t_list *node  = promp->cmds;
    t_mini **arr;
    pid_t  *pids;
    int   **pipes;
    int      i;

    // Count commands
    while (node)
    {
        count++;
        node = node->next;
    }
    if (count == 0)
        return;

    arr  = malloc(sizeof(t_mini*) * count);
    pids = malloc(sizeof(pid_t)    * count);
    pipes = malloc(sizeof(int*) * (count - 1));
    if (!arr || !pids || (count > 1 && !pipes))
    {
        free(arr); free(pids); free(pipes);
        return;
    }

    node = promp->cmds;
    for (i = 0; i < count; i++)
    {
        arr[i] = node->content;
        node = node->next;
    }

    if (count == 1 && is_builtin(arr[0]->full_cmd))
    {
        int saved_stdin = dup(STDIN_FILENO);
        int saved_stdout = dup(STDOUT_FILENO);

        int redir_status = setup_redirections(arr[0]);
        if (redir_status)
        {
            *(promp->err_code) = 1;
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);
            free(arr);
            free(pids);
            free(pipes);
            return;
        }
        *(promp->err_code) = exec_builtin(arr[0], &promp->tenv);

        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        free(arr);
        free(pids);
        free(pipes);
        return;
    }

    // Create pipes
    for (i = 0; i < count - 1; i++)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i] || pipe(pipes[i]) < 0)
        {
            perror("pipe");
            for (int j = 0; j <= i; j++) free(pipes[j]);
            free(arr); free(pids); free(pipes);
            exit(1);
        }
    }

    // Fork children
    for (i = 0; i < count; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        if (pid == 0)
        {
            signal(SIGINT, SIG_DFL); // gecisi kapalı.
            if (i > 0)
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < count - 1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            // Close all pipe fds in child
            for (int j = 0; j < count - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
        
            if (setup_redirections(arr[i]))
            {
                exit(1); // Redirection error
            }

            if (is_builtin(arr[i]->full_cmd))
            {
                int ret = exec_builtin(arr[i], &promp->tenv);
                exit(ret);
            }
            execute_cmd(arr[i], promp->envp);
            exit(1); // Should not reach here
        }
        pids[i] = pid;
    }

    // Parent closes all pipe fds
    for (i = 0; i < count - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        free(pipes[i]);
    }
    free(pipes);
    for (i = 0; i < count; i++)
    {
        int wstatus;
        waitpid(pids[i], &wstatus, 0);
        if (i == count - 1)
        {
            if (WIFEXITED(wstatus))
                *(promp->err_code) = WEXITSTATUS(wstatus);
            else if (WIFSIGNALED(wstatus))
                *(promp->err_code) = 128 + WTERMSIG(wstatus);
        }
    }

    free(arr);
    free(pids);
}
// ...existing code...
