/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:31:33 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/02 05:01:09 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_operator(t_token_type type)
{
    return (type == T_PIPE || type == T_REDIR_IN
        || type == T_REDIR_OUT || type == T_REDIR_APPEND
        || type == T_HEREDOC);
}

static int  print_syntax_error(t_token *token)
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

static int  is_redirection(int type)
{
    return (type == T_REDIR_IN || type == T_REDIR_OUT
        || type == T_REDIR_APPEND || type == T_HEREDOC);
}

int check_syntax(t_token *head)
{
    t_token *current = head; // head'i değiştirmemek için ayrı bir pointer kullanın

    if (!current)
        return (1);
    if (current->type == T_PIPE)
        return (print_syntax_error(current));
    while (current)
    {
        if (current->type == T_PIPE)
        {
            if (!current->next) // Pipe en sonda olamaz
                return (print_syntax_error(NULL)); // 'newline' hatası
            if (current->next->type == T_PIPE) // '||' hatası
                return (print_syntax_error(current->next));
        }
        else if (is_redirection(current->type))
        {
            if (!current->next) // Redirection en sonda olamaz
                return (print_syntax_error(NULL)); // 'newline' hatası
            if (is_operator(current->next->type))
                return (print_syntax_error(current->next));
        }
        current = current->next;
    }
    return (0);
}
