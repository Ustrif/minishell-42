/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:31:33 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/01 16:37:55 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(t_token_type type)
{
	return (type == T_PIPE || type == T_REDIR_IN
		|| type == T_REDIR_OUT || type == T_REDIR_APPEND
		|| type == T_HEREDOC);
}

static int	print_syntax_error(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	if (!token)
		ft_putendl_fd("newline'", 2);
	else
	{
		ft_putstr_fd(token->value, 2);
		ft_putendl_fd("'", 2);
	}
	return (1);
}

static int	is_redirection(int type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT
		|| type == T_REDIR_APPEND || type == T_HEREDOC);
}

int	check_syntax(t_token *head)
{
	if (!head)
		return (1);
	if (head->type == T_PIPE)
		return (print_syntax_error(head));
	while (head)
	{
		if (head->type == T_PIPE
			&& (!head->next || is_operator(head->next->type)))
			return (print_syntax_error(head->next));
		if (is_redirection(head->type)
			&& (!head->next || is_operator(head->next->type)))
			return (print_syntax_error(head->next));
		head = head->next;
	}
	return (0);
}

/*
int	check_syntax(t_token *head)
{
	if (!head)
		return (1);
	if (is_operator(head->type) && head->type != T_HEREDOC)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
		ft_putstr_fd(head->value, 2);
		ft_putendl_fd("'", 2);
		return (1);
	}
	while (head)
	{
		if (head->type == T_PIPE && (!head->next || is_operator(head->next->type)))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
			if (!head->next)
				ft_putendl_fd("newline'", 2);
			else
			{
				ft_putstr_fd(head->next->value, 2);
				ft_putendl_fd("'", 2);
			}
			return (1);
		}
		if ((head->type == T_REDIR_IN || head->type == T_REDIR_OUT
			|| head->type == T_REDIR_APPEND || head->type == T_HEREDOC)
			&& (!head->next || is_operator(head->next->type)))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
			if (!head->next)
				ft_putendl_fd("newline'", 2);
			else
			{
				ft_putstr_fd(head->next->value, 2);
				ft_putendl_fd("'", 2);
			}
			return (1);
		}
		head = head->next;
	}
	return (0);
}
*/