/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:01:10 by raydogmu          #+#    #+#             */
/*   Updated: 2025/06/28 20:08:18 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	*cleanup(t_mini *mini, char **full_cmd, t_list **cmds)
{
	if (full_cmd)
		free_all(full_cmd);
	if (mini)
		free_mini(mini);
	if (cmds)
		ft_lstclear(cmds, free_mini);
	return (NULL);
}

void	*job(t_token **tokens, char ***full_cmd, t_mini **mini, t_list **cmds)
{
	if ((*tokens)->type == T_WORD)
	{
		*full_cmd = get_swords(*full_cmd, (*tokens)->value);
		if (!(*full_cmd))
			return (cleanup(*mini, *full_cmd, cmds));
	}
	else if ((*tokens)->type == T_REDIR_IN)
	{
		(*mini)->infile = open((*tokens)->next->value, O_RDONLY);
		if ((*mini)->infile < 0)
		{
			g_status = 1;
			perror((*tokens)->next->value);
			return (cleanup(*mini, *full_cmd, cmds));
		}
		(*tokens) = (*tokens)->next;
	}
	return ((void *)1);
}

void	*job1(t_token **tokens, char ***full_cmd, t_mini **mini, t_list **cmds)
{
	if ((*tokens)->type == T_REDIR_OUT)
	{
		(*mini)->outfile = open((*tokens)->next->value,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if ((*mini)->outfile < 0)
			return (cleanup(*mini, *full_cmd, cmds));
		*tokens = (*tokens)->next;
	}
	else if ((*tokens)->type == T_HEREDOC)
	{
		(*mini)->infile = open_heredoc(*tokens);
		if ((*mini)->infile < 0)
			return (cleanup(*mini, *full_cmd, cmds));
		*tokens = (*tokens)->next;
	}
	return ((void *)1);
}

void	*job2(t_token **tokens, char ***full_cmd, t_mini **mini, t_list **cmds)
{
	t_list	*node;

	if ((*tokens)->type == T_PIPE)
	{
		(*mini)->full_cmd = *full_cmd;
		node = ft_lstnew(*mini);
		if (!node)
			return (cleanup(*mini, *full_cmd, cmds));
		ft_lstadd_back(cmds, node);
		(*mini) = init_mini();
		if (!(*mini))
			return (cleanup(*mini, *full_cmd, cmds));
		*full_cmd = NULL;
	}
	else if ((*tokens)->type == T_REDIR_APPEND)
	{
		(*mini)->outfile = open((*tokens)->next->value,
				O_CREAT | O_WRONLY | O_APPEND, 0644);
		if ((*mini)->outfile < 0)
			return (cleanup(*mini, *full_cmd, cmds));
		*tokens = (*tokens)->next;
	}
	return ((void *)1);
}

t_list	*get_minis(t_token *tokens)
{
	t_list	*cmds;
	t_mini	*mini;
	char	**full_cmd;
	t_list	*node;

	cmds = NULL;
	mini = init_mini();
	full_cmd = NULL;
	node = NULL;
	if (!mini)
		return (NULL);
	while (tokens)
	{
		if (job(&tokens, &full_cmd, &mini, &cmds) == NULL
			|| job1(&tokens, &full_cmd, &mini, &cmds) == NULL
			|| job2(&tokens, &full_cmd, &mini, &cmds) == NULL)
			return (NULL);
		tokens = tokens->next;
	}
	mini->full_cmd = full_cmd;
	node = ft_lstnew(mini);
	if (!node)
		return (cleanup(mini, full_cmd, &cmds));
	ft_lstadd_back(&cmds, node);
	return (cmds);
}
