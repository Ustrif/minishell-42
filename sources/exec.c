/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 03:04:26 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/02 05:38:27 by raydogmu         ###   ########.fr       */
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

void execute_cmd(t_mini *cmd, char **envp)
{
	struct stat sb;

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

void setup_redirections(t_mini *cmd)
{
    t_redir *r = cmd->redir;
    int      fd;

    signal(SIGINT, SIG_DFL);
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
            perror(r->target), exit(1);

        if (r->type == R_IN || r->type == R_HEREDOC)
            dup2(fd, STDIN_FILENO);
        else
            dup2(fd, STDOUT_FILENO);

        close(fd);
        r = r->next;
    }
}

void execute_pipeline(t_promp *promp)
{
    int     count = 0;
    t_list *node  = promp->cmds;
    t_mini **arr;
    pid_t  *pids;
    int      i;

    while (node)
    {
        count++;
        node = node->next;
    }
    t_mini *only = promp->cmds->content;
    if (count == 1 && is_builtin(only->full_cmd) && ft_strcmp(only->full_cmd[0], "exit") == 0)
    {
        setup_redirections(only);

        if (ft_strcmp(only->full_cmd[0], "exit") == 0)
        {
            int code = exec_builtin(only, &promp->tenv);
            exit(code);
        }
        int ret = exec_builtin(only, &promp->tenv);
        *(promp->err_code) = ret;
        return ;
    }
    arr  = malloc(sizeof(t_mini*) * count);
    pids = malloc(sizeof(pid_t)    * count);
    if (!arr || !pids)
        return;
    node = promp->cmds;
    for (i = 0; i < count; i++)
    {
        arr[i] = node->content;
        node = node->next;
    }

    // 2) Bütün pipe’ları aç (count-1 tane)
    int pipes[count - 1][2];
    for (i = 0; i < count - 1; i++)
    {
        if (pipe(pipes[i]) < 0)
        {
            perror("pipe");
            exit(1);
        }
    }

    // 3) Sağdan sola fork
    for (i = count - 1; i >= 0; i--)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        if (pid == 0)
        {
            // --- CHILD ---

            // a) PIPE’ları bağla
            signal(SIGPIPE, SIG_IGN);
            if (i < count - 1)
            {
                // stdout → sağdaki komutun okuyacağı pipe’a
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            if (i > 0)
            {
                // stdin ← soldaki komutun yazdığı pipe’dan
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            // artık pipe fd’leri gereksiz
            for (int j = 0; j < count - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // b) SONRA redir’ları kur (<, >, >>, heredoc vs.)
            setup_redirections(arr[i]);

			if (is_builtin(arr[i]->full_cmd))
            {
                int ret = exec_builtin(arr[i], &promp->tenv);
                exit(ret);
            }
            // c) exec
            execute_cmd(arr[i], promp->envp);
            // hata olursa execute_cmd içinde exit() edilir
        }
        // PARENT:
        pids[i] = pid;
    }

    // 4) PARENT: pipe’ları kapat
    for (i = 0; i < count - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // 5) Tüm CHILD’ları bekle, kodu son komuta göre ayarla
    {
        int status = 0;
        for (i = 0; i < count; i++)
            waitpid(pids[i], &status, 0);

        if (WIFEXITED(status))
            *(promp->err_code) = WEXITSTATUS(status);
        else
            *(promp->err_code) = 128 + WTERMSIG(status);
    }

    free(arr);
    free(pids);
}
