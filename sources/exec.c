/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:01:38 by codespace         #+#    #+#             */
/*   Updated: 2025/07/01 16:54:08 by codespace        ###   ########.fr       */
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
	head = get_expanded_tokens(head, *err_code);
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

void	child_exec(t_mini *mini, char **envp, int prev_in, int pipe_out)
{
	// Restore default signal handling in child processes
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	
	// Set up input redirection
	if (mini->infile != STDIN_FILENO)
	{
		if (dup2(mini->infile, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		close(mini->infile);
	}
	else if (prev_in != -1)
	{
		if (dup2(prev_in, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		close(prev_in);
	}
	
	// Set up output redirection
	if (mini->outfile != STDOUT_FILENO)
	{
		if (dup2(mini->outfile, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		close(mini->outfile);
	}
	else if (pipe_out != -1)
	{
		if (dup2(pipe_out, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		close(pipe_out);
	}
	
	// Handle empty command
	if (!mini->full_cmd || !mini->full_cmd[0])
		exit(0);
	
	// Handle command not found
	if (!mini->full_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(mini->full_cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	if (isdirectory(mini->full_cmd))
		exit(126);
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

static int	count_commands(t_list *cmds)
{
	int count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

static int	is_single_builtin(t_promp *prompt)
{
	t_list *cmds = prompt->cmds;
	t_mini *mini;
	
	// Check if there's only one command and it's a builtin
	if (!cmds || cmds->next)
		return (0);
	
	mini = (t_mini *)cmds->content;
	return (is_builtin(mini->full_cmd));
}


void	execute_cmds(t_promp *prompt)
{
	t_list	*curr;
	int		**pipes;
	pid_t	*pids;
	int		cmd_count;
	int		i;
	int		j;
	int		status;
	t_mini	*mini;
	
	if (!prompt || !prompt->cmds)
		return ;
	
	cmd_count = count_commands(prompt->cmds);
		if (is_single_builtin(prompt))
	{
		mini = (t_mini *)prompt->cmds->content;
		
		// Save and redirect stdin/stdout if needed
		int saved_stdin = -1, saved_stdout = -1;
		
		if (mini->infile != STDIN_FILENO)
		{
			saved_stdin = dup(STDIN_FILENO);
			dup2(mini->infile, STDIN_FILENO);
			close(mini->infile);
		}
		
		if (mini->outfile != STDOUT_FILENO)
		{
			saved_stdout = dup(STDOUT_FILENO);
			dup2(mini->outfile, STDOUT_FILENO);
			close(mini->outfile);
		}
		
		*prompt->err_code = exec_builtin(mini, &prompt->tenv);
		
		// Restore stdin/stdout
		if (saved_stdin != -1)
		{
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
		}
		if (saved_stdout != -1)
		{
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdout);
		}
		return ;
	}
	
	// Allocate arrays for pipes and pids
	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	pids = malloc(sizeof(pid_t) * cmd_count);
	
	if (!pipes || !pids)
	{
		if (pipes) free(pipes);
		if (pids) free(pids);
		return ;
	}
	
	// Create all pipes
	for (i = 0; i < cmd_count - 1; i++)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			// Clean up already created pipes
			for (j = 0; j < i; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				free(pipes[j]);
			}
			free(pipes);
			free(pids);
			return ;
		}
	}
	
	// Fork all processes
	curr = prompt->cmds;
	for (i = 0; i < cmd_count; i++)
	{
		mini = (t_mini *)curr->content;
		
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			// Kill already forked processes
			for (j = 0; j < i; j++)
				kill(pids[j], SIGTERM);
			// Clean up pipes
			for (j = 0; j < cmd_count - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				free(pipes[j]);
			}
			free(pipes);
			free(pids);
			return ;
		}
		else if (pids[i] == 0)
		{
			// Child process
			// Set up input
			if (i > 0)
			{
				dup2(pipes[i - 1][0], STDIN_FILENO);
			}
			
			// Set up output
			if (i < cmd_count - 1)
			{
				dup2(pipes[i][1], STDOUT_FILENO);
			}
			
			// Close all pipe file descriptors
			for (j = 0; j < cmd_count - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			
			// Execute command
			if (is_builtin(mini->full_cmd))
			{
				// Handle file redirections for builtins
				if (mini->infile != STDIN_FILENO && i == 0)
					dup2(mini->infile, STDIN_FILENO);
				if (mini->outfile != STDOUT_FILENO && i == cmd_count - 1)
					dup2(mini->outfile, STDOUT_FILENO);
				
				exit(exec_builtin(mini, &prompt->tenv));
			}
			else
			{
				child_exec(mini, prompt->envp, -1, -1);
			}
		}
		
		curr = curr->next;
	}
	
	// Close all pipes in parent
	for (i = 0; i < cmd_count - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
	}
	free(pipes);
	
	// Wait for all children
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)  // Last command sets the exit status
		{
			if (WIFEXITED(status))
				*prompt->err_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*prompt->err_code = 128 + WTERMSIG(status);
		}
	}
	
	free(pids);
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