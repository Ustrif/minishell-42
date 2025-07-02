/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:01:10 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/02 02:20:23 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char	*line;

	if ((*tokens)->type == T_WORD)
	{
		*full_cmd = get_swords(*full_cmd, (*tokens)->value);
		if (!(*full_cmd))
			return (cleanup(*mini, *full_cmd, cmds));
	}
	else if ((*tokens)->type == T_REDIR_IN)
	{
		line = ft_strdup((*tokens)->next->value);
		(*mini)->infilec = get_swords((*mini)->infilec, line);
		free(line);
		if ((*mini)->infilec == NULL)
		{
			perror((*tokens)->next->value);
			return (cleanup(*mini, *full_cmd, cmds));
		}
		(*tokens) = (*tokens)->next;
	}
	return ((void *)1);
}

void	*job1(t_token **tokens, char ***full_cmd, t_mini **mini, t_list **cmds)
{
	char	*line;

	if ((*tokens)->type == T_REDIR_OUT)
	{
		line = ft_strdup((*tokens)->next->value);
		(*mini)->outfilec = get_swords((*mini)->outfilec, line);
		free(line);
		if ((*mini)->outfilec == NULL)
		{
			perror((*tokens)->next->value);
			return (cleanup(*mini, *full_cmd, cmds));
		}
		*tokens = (*tokens)->next;
	}
	else if ((*tokens)->type == T_HEREDOC)
	{
		line = ft_strdup((*tokens)->value);
		(*mini)->heredoc = get_swords((*mini)->heredoc, line);
		free(line);
		if ((*mini)->heredoc == NULL)
		{
			perror((*tokens)->next->value);
			return (cleanup(*mini, *full_cmd, cmds));
		}
		*tokens = (*tokens)->next;
	}
	return ((void *)1);
}

void	*job2(t_token **tokens, char ***full_cmd, t_mini **mini, t_list **cmds)
{
	t_list	*node;
	char	*line;

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
		line = ft_strdup((*tokens)->next->value);
		(*mini)->appendfilec = get_swords((*mini)->appendfilec, line);
		free(line);
		if ((*mini)->appendfilec == NULL)
			return (perror((*tokens)->next->value),
				cleanup(*mini, *full_cmd, cmds));
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
		if (!job(&tokens, &full_cmd, &mini, &cmds)
			|| !job1(&tokens, &full_cmd, &mini, &cmds)
			|| !job2(&tokens, &full_cmd, &mini, &cmds))
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
