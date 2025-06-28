/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:01:38 by codespace         #+#    #+#             */
/*   Updated: 2025/06/28 19:56:46 by raydogmu         ###   ########.fr       */
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

t_promp	*get_full_promp(char *s, char **env)
{
	t_token	*head;
	t_list	*minis;
	t_promp	*p;

	head = get_classified_tokens(s);
	head = get_expanded_tokens(head);
	head = get_unqouted_tokens(head);
	if (!head)
		return (printf("quote err"), NULL);
	minis = get_full_minis(head, env);
	ft_tokenclear(&head);
	if (!minis)
	{
		ft_tokenclear(&head);
		return (NULL);
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
		g_status = WEXITSTATUS(status);
}
