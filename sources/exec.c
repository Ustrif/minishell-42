/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:01:38 by codespace         #+#    #+#             */
/*   Updated: 2025/07/02 03:48:14 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

char	**get_fenv(char **e)
{
	char	**result;
	int		i;
	int		env_num;
	
	result = NULL;
	i = 0;
	env_num = count_words(e);
	while (e[i])
	{
		result = get_swords(result, e[i]);
		i++;
	}
	if (env_num != count_words(result))
	{
		free_all(result);
		return (NULL);
	}
	return (result);
}

t_promp	*get_full_promp(char *s, char **env, int *err_code)
{
	t_token	*head;
	t_list	*minis;
	t_promp	*p;
	
	head = get_classified_tokens(s);
	head = get_unqouted_tokens(head);
	if (!head)
		return (*err_code = 1, NULL);
	minis = get_full_minis(head, env);
	ft_tokenclear(&head);
	if (!minis)
	{
		*err_code = 1;
		return (NULL);
	}
	p = get_prompt(minis, env);
	if (!p)
	{
		*err_code = 1;
		ft_lstclear(&minis, free_mini);
		return (NULL);
	}
	return (p);
}

int	is_builtin(char **args)
{
	if (!args || !args[0])
		return (0);
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (1);
	else if (ft_strcmp(args[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(args[0], "env") == 0)
		return (1);
	else if (ft_strcmp(args[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(args[0], "export") == 0)
		return (1);
	else if (ft_strcmp(args[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(args[0], "exit") == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_mini *mini, t_env **env_list)
{
	if (!mini->full_cmd || !mini->full_cmd[0])
		return (1);
	else if (ft_strcmp(mini->full_cmd[0], "pwd") == 0)
		return (get_pwd());
	else if (ft_strcmp(mini->full_cmd[0], "echo") == 0)
		return (ft_echo(mini->full_cmd));
	else if (ft_strcmp(mini->full_cmd[0], "env") == 0)
		return (ft_env(mini->full_cmd, *env_list));
	else if (ft_strcmp(mini->full_cmd[0], "cd") == 0)
		return (get_cd(mini->full_cmd));
	else if (ft_strcmp(mini->full_cmd[0], "export") == 0)
		return (command_export(env_list, mini->full_cmd));
	else if (ft_strcmp(mini->full_cmd[0], "unset") == 0)
		return (command_unset(mini->full_cmd, env_list));
	else if (ft_strcmp(mini->full_cmd[0], "exit") == 0)
		return (ft_exit(mini->full_cmd));
	return (-1);
}

int	isdirectory(char **full_path)
{
	struct stat	sb;

	if (!full_path)
		return (0);
	if (stat(full_path[0], &sb) == 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(full_path[0], STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}
#include "minishell.h"

static void	handle_pipes(int **pipes, int cmd_count);
static void	execute_single_builtin(t_promp *prompt, t_mini *mini);
static void	execute_pipeline(t_promp *prompt, int cmd_count);
static void	cleanup_pipes(int **pipes, int cmd_count);
static void	wait_for_children(pid_t *pids, int cmd_count, int *err_code);

void	execute_cmds(t_promp *prompt)
{
	int		cmd_count;
	t_list	*cmds;

	if (!prompt || !prompt->cmds)
		return ;
	cmd_count = ft_lstsize(prompt->cmds);
	cmds = prompt->cmds;
	if (cmd_count == 1 && is_builtin(((t_mini *)cmds->content)->full_cmd))
		execute_single_builtin(prompt, (t_mini *)cmds->content);
	else
		execute_pipeline(prompt, cmd_count);
}

static void	execute_single_builtin(t_promp *prompt, t_mini *mini)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (mini->infile != STDIN_FILENO)
	{
		dup2(mini->infile, STDIN_FILENO);
		close(mini->infile);
	}
	if (mini->outfile != STDOUT_FILENO)
	{
		dup2(mini->outfile, STDOUT_FILENO);
		close(mini->outfile);
	}
	*prompt->err_code = exec_builtin(mini, &prompt->tenv);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

static void	execute_pipeline(t_promp *prompt, int cmd_count)
{
	int		**pipes;
	pid_t	*pids;
	t_list	*curr;
	int		i;

	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pipes || !pids)
		return (free(pipes), free(pids));
	handle_pipes(pipes, cmd_count);
	i = -1;
	curr = prompt->cmds;
	while (++i < cmd_count && curr)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			child_process(prompt, (t_mini *)curr->content, pipes, i, cmd_count);
		else if (pids[i] < 0)
			break ;
		curr = curr->next;
	}
	cleanup_pipes(pipes, cmd_count);
	wait_for_children(pids, cmd_count, prompt->err_code);
	free(pids);
	free(pipes);
}

void	child_process(t_promp *prompt, t_mini *mini, int **pipes, int i, int cmd_count)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	cleanup_pipes(pipes, cmd_count);
	if (mini->infile != STDIN_FILENO)
	{
		dup2(mini->infile, STDIN_FILENO);
		close(mini->infile);
	}
	if (mini->outfile != STDOUT_FILENO)
	{
		dup2(mini->outfile, STDOUT_FILENO);
		close(mini->outfile);
	}
	if (is_builtin(mini->full_cmd))
		exit(exec_builtin(mini, &prompt->tenv));
	else if (mini->full_path && access(mini->full_path, X_OK) == 0)
		execve(mini->full_path, mini->full_cmd, prompt->envp);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(mini->full_cmd[0], STDERR_FILENO);
	if (!mini->full_path)
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	else
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	exit(mini->full_path ? 126 : 127);
}

static void	handle_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = -1;
	while (++i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("minishell");
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			exit(1);
		}
	}
}

static void	cleanup_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = -1;
	while (++i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
	}
}

static void	wait_for_children(pid_t *pids, int cmd_count, int *err_code)
{
	int	status;
	int	i;

	i = -1;
	while (++i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				*err_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*err_code = 128 + WTERMSIG(status);
		}
	}
}

/* 
 * You'll also need to add proper signal handling in your main shell loop.
 * Add this to your main function or initialization:
 */

/*
void	handle_sigint(int sig)
{
	(void)sig;
	g_status = 130;  // Ctrl+C exit status
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// In your main function, add:
signal(SIGINT, handle_sigint);
signal(SIGQUIT, SIG_IGN);  // Ignore Ctrl+\ in interactive mode
*/

/*
char	**get_fenv(char **e)
{
	char	**result;
	int		i;
	int		env_num;

	result = NULL;
	i = 0;
	env_num = count_words(e);
	while (e[i])
	{
		result = get_swords(result, e[i]);
		i++;
	}
	if (env_num != count_words(result))
	{
		free_all(result);
		return (NULL);
	}
	return (result);
}

t_promp	*get_full_promp(char *s, char **env, int *err_code)
{
	t_token	*head;
	t_list	*minis;
	t_promp	*p;

	head = get_classified_tokens(s);
	head = get_expanded_tokens(head, *err_code);
	head = get_unqouted_tokens(head);
	if (!head)
		return (*err_code = 1, NULL);
	minis = get_full_minis(head, env);
	ft_tokenclear(&head);
	if (!minis)
	{
		*err_code = 2;
		return (NULL);
	}
	p = get_prompt(minis, env);
	if (!p)
	{
		*err_code = 1;
		ft_lstclear(&minis, free_mini);
		return (NULL);
	}
	return (p);
}

int	is_builtin(char **args)
{
	if (!args || !args[0])
		return (0);
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (1);
	else if (ft_strcmp(args[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(args[0], "env") == 0)
		return (1);
	else if (ft_strcmp(args[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(args[0], "export") == 0)
		return (1);
	else if (ft_strcmp(args[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(args[0], "exit") == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_mini *mini, t_env **env_list)
{
	if (!mini->full_cmd || !mini->full_cmd[0])
		return (1);
	else if (ft_strcmp(mini->full_cmd[0], "pwd") == 0)
		return (get_pwd());
	else if (ft_strcmp(mini->full_cmd[0], "echo") == 0)
		return (ft_echo(mini->full_cmd));
	else if (ft_strcmp(mini->full_cmd[0], "env") == 0)
		return (ft_env(mini->full_cmd, *env_list));
	else if (ft_strcmp(mini->full_cmd[0], "cd") == 0)
		return (get_cd(mini->full_cmd));
	else if (ft_strcmp(mini->full_cmd[0], "export") == 0)
		return (command_export(env_list, mini->full_cmd));
	else if (ft_strcmp(mini->full_cmd[0], "unset") == 0)
		return (command_unset(mini->full_cmd, env_list));
	else if (ft_strcmp(mini->full_cmd[0], "exit") == 0)
		return (ft_exit(mini->full_cmd));
	return (-1);
}

void	child_exec(t_mini *mini, char **envp, int prev_in, int pipe_out)
{
	if (mini->infile != STDIN_FILENO)
	{
		dup2(mini->infile, STDIN_FILENO);
		close(mini->infile);
	}
	else if (prev_in != -1)
	{
		dup2(prev_in, STDIN_FILENO);
		close(prev_in);
	}
	if (mini->outfile != STDOUT_FILENO)
	{
		dup2(mini->outfile, STDOUT_FILENO);
		close(mini->outfile);
	}
	else if (pipe_out != -1)
	{
		dup2(pipe_out, STDOUT_FILENO);
		close(pipe_out);
	}
	if (pipe_out != -1)
		close(pipe_out);
	if (!mini->full_cmd || !mini->full_cmd[0])
		exit(0);
	if (!mini->full_path)
	{
		printf("minishell: %s: command not found\n", mini->full_cmd[0]);
		exit(127);
	}
	if (execve(mini->full_path, mini->full_cmd, envp) == -1)
	{
		perror("minishell");
		if (errno == EACCES || errno == ENOTDIR || errno == ENOEXEC)
			exit(126);
		else if (errno == ENOENT)
			exit(127);
		else
			exit(1);
	}
}

void	execute_cmds(t_promp *prompt)
{
	t_list	*curr;
	int		pipefd[2];
	int		prev_in;
	pid_t	pid;
	int		status;
	t_mini	*mini;

	if (!prompt || !prompt->cmds)
		return ;
	curr = prompt->cmds;
	prev_in = -1;
	while (curr)
	{
		mini = (t_mini *)curr->content;
		if (curr->next && pipe(pipefd) == -1)
			return (perror("pipe"));
		if (mini && is_builtin(mini->full_cmd))
		{
			if (mini->infile != STDIN_FILENO)
				dup2(mini->infile, STDIN_FILENO);
			if (mini->outfile != STDOUT_FILENO)
				dup2(mini->outfile, STDOUT_FILENO);
			*prompt->err_code = exec_builtin(mini, &prompt->tenv);
		}
		else
		{
			pid = fork();
			if (pid == -1)
				return (perror("fork"));
			else if (pid == 0)
				child_exec(mini, prompt->envp, prev_in, curr->next ? pipefd[1] : -1);
			else
			{
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					*prompt->err_code = WEXITSTATUS(status);
			}
		}
		if (prev_in != -1)
			close(prev_in);
		if (curr->next)
		{
			close(pipefd[1]);
			prev_in = pipefd[0];
		}
		curr = curr->next;
	}
}



void	execute_cmds(t_promp *prompt)
{
	t_list	*curr;
	int		pipefd[2];
	int		prev_in = -1;
	pid_t	pid;
	int		status;

	if (!prompt || !prompt->cmds)
		return;

	curr = prompt->cmds;
	while (curr)
	{
		t_mini *mini = (t_mini *)curr->content;

		// Eğer bir sonraki komut varsa pipe oluştur
		if (curr->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			return;
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return;
		}
		else if (pid == 0)
		{
			// infile
			if (mini->infile != STDIN_FILENO)
			{
				dup2(mini->infile, STDIN_FILENO);
				close(mini->infile);
			}
			else if (prev_in != -1)
			{
				dup2(prev_in, STDIN_FILENO);
				close(prev_in);
			}

			// outfile
			if (mini->outfile != STDOUT_FILENO)
			{
				dup2(mini->outfile, STDOUT_FILENO);
				close(mini->outfile);
			}
			else if (curr->next)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}

			if (pipefd[0])
				close(pipefd[0]);

			if (!mini->full_cmd || !mini->full_cmd[0])
				exit(0);
			if (execve(mini->full_path, mini->full_cmd, prompt->envp) == -1)
			{
				perror("minishell");
				exit(127);
			}
		}
		else
		{
			waitpid(pid, &status, 0);
			if (prev_in != -1)
				close(prev_in);
			if (curr->next)
			{
				close(pipefd[1]);
				prev_in = pipefd[0];
			}
			curr = curr->next;
		}
	}
	if (WIFEXITED(status))
		g_status = WEXITSTATUS(status); // g_status değil, errcode
}



*/