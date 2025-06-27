/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:01:38 by codespace         #+#    #+#             */
/*   Updated: 2025/06/27 17:02:18 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_promp *get_full_promp(char *s, char **env)
{
    t_token	*head;
    t_list  *minis;
    t_promp *p;

	head = get_classified_tokens(s);
	if (!head)
		return (printf("head err"), NULL);
	head = get_unqouted_tokens(head);
	if (!head)
		return (printf("quote err"), NULL);
	minis = get_full_minis(head, env);
	ft_tokenclear(&head);
    if (!minis)
    {
        ft_tokenclear(&head);
        return (printf("s yok"), NULL);
    }
    p = get_prompt(minis, env);
    if (!p)
    {
        ft_tokenclear(&head);
        ft_lstclear(&minis, free_mini);
        return (printf("p"), NULL);
    }
	return (p);
}

void execute_command(t_mini *mini, char **envp)
{
	if (!mini || !mini->full_cmd || !mini->full_cmd[0])
		exit(0);
	else
		execve(mini->full_path, mini->full_cmd, envp);
	perror("execve");
	exit(127);
}

int exec_minishell(t_list *cmds, char **envp)
{
	int		pipefd[2];
	int		prev_fd = -1;
	t_mini	*mini;
	pid_t	pid;

	while (cmds)
	{
		mini = (t_mini *)cmds->content;
		if (cmds->next)
			pipe(pipefd);
		pid = fork();
		if (pid == 0)
		{
			if (mini->infile != STDIN_FILENO)
				dup2(mini->infile, STDIN_FILENO);
			else if (prev_fd != -1)
				dup2(prev_fd, STDIN_FILENO);

			if (mini->outfile != STDOUT_FILENO)
				dup2(mini->outfile, STDOUT_FILENO);
			else if (cmds->next)
				dup2(pipefd[1], STDOUT_FILENO);

			if (prev_fd != -1)
				close(prev_fd);
			if (cmds->next)
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			execute_command(mini, envp);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		cmds = cmds->next;
	}
	while (wait(NULL) > 0);
	return (0);
}


int	main(int argc, char **argv, char **env)
{
	char	*line;
    t_promp *p;
	char	**fenv;

	fenv = get_fenv(env);
    if (!argc || !argv || !fenv)
		return (1);
	//line = "e'c'ho \"hello      there\" how are 'you 'doing? $USER |wc -l >outfile";
	line = "cat << eof | grep 'x' | wc -l ";
	//line = "cat << eof";
	//line = "cat | cat | ls";
	//line = "cat < a.txt | wc -l > num.txt";
	//line = "echo \"$USER\"'$USER'";
	p = get_full_promp(line, fenv);
    if (!p)
        return (printf("fullp error."), 1);
    exec_minishell(p->cmds, p->envp);
	del_prompt(p, free_mini);
    return (0);
}
