/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 14:58:43 by beinan            #+#    #+#             */
/*   Updated: 2025/04/19 14:58:43 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	select_token_type(t_token *token)
{
	if (!token || !token->value)
		return ;
	if (ft_strncmp(token->value, "|", 2) == 0)
		token->type = T_PIPE;
	else if (ft_strncmp(token->value, "<", 2) == 0)
		token->type = T_REDIR_IN;
	else if (ft_strncmp(token->value, ">", 2) == 0)
		token->type = T_REDIR_OUT;
	else if (ft_strncmp(token->value, ">>", 3) == 0)
		token->type = T_REDIR_APPEND;
	else if (ft_strncmp(token->value, "<<", 3) == 0)
		token->type = T_HEREDOC;
	else
		token->type = T_WORD;
}

void	classify_all_tokens(t_token *tokens)
{
	while (tokens)
	{
		select_token_type(tokens);
		tokens = tokens->next;
	}
}

t_token	*get_classified_tokens(char	*line)
{
	t_token	*head;

	head = get_tokens(line);
	if (!head)
		return (NULL);
	classify_all_tokens(head);
	return (head);
}
