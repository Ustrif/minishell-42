/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:12:38 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/12 15:28:07 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	*job0(t_token **tok, char ***full_cmd,
		t_mini **mini, t_list **cmds)
{
	if ((*tok)->type == T_WORD)
	{
		*full_cmd = get_swords(*full_cmd, (*tok)->value);
		if (!*full_cmd)
			return (cleanup(*mini, *full_cmd, cmds));
	}
	else if ((*tok)->type == T_REDIR_IN)
	{
		if (!append_redir(&(*mini)->redir, R_IN, (*tok)->next->value))
			return (cleanup(*mini, *full_cmd, cmds));
		*tok = (*tok)->next;
	}
	return ((void *)1);
}

static void	*job1(t_token **tok, char ***full_cmd, t_mini **mini, t_list **cmds)
{
	if ((*tok)->type == T_REDIR_OUT)
	{
		if (!append_redir(&(*mini)->redir, R_OUT, (*tok)->next->value))
			return (cleanup(*mini, *full_cmd, cmds));
		*tok = (*tok)->next;
	}
	if ((*tok)->type == T_REDIR_APPEND)
	{
		if (!append_redir(&(*mini)->redir, R_APPEND, (*tok)->next->value))
			return (cleanup(*mini, *full_cmd, cmds));
		*tok = (*tok)->next;
	}
	return ((void *)1);
}

static void	*job2(t_token **tok, char ***full_cmd,
		t_mini **mini, t_list **cmds)
{
	t_list	*node;

	if ((*tok)->type == T_HEREDOC)
	{
		if (!append_redir(&(*mini)->redir, R_HEREDOC, (*tok)->next->value))
			return (cleanup(*mini, *full_cmd, cmds));
		*tok = (*tok)->next;
	}
	if ((*tok)->type == T_PIPE)
	{
		(*mini)->full_cmd = *full_cmd;
		node = ft_lstnew(*mini);
		if (!node)
			return (cleanup(*mini, *full_cmd, cmds));
		ft_lstadd_back(cmds, node);
		*mini = init_mini();
		*full_cmd = NULL;
		if (!*mini)
			return (cleanup(*mini, *full_cmd, cmds));
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
	if (!mini)
		return (NULL);
	while (tokens)
	{
		if ((job0(&tokens, &full_cmd, &mini, &cmds) == NULL)
			|| (job1(&tokens, &full_cmd, &mini, &cmds) == NULL)
			|| (job2(&tokens, &full_cmd, &mini, &cmds) == NULL))
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
