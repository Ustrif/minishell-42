/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:01:10 by raydogmu          #+#    #+#             */
/*   Updated: 2025/07/02 04:25:27 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int append_redir(t_redir **list, t_redir_type type, const char *target)
{
    t_redir *new = malloc(sizeof(t_redir));
    if (!new) return 0;
    new->type = type;
    new->target = ft_strdup(target);
    if (!new->target) { free(new); return 0; }
    new->next = NULL;
    if (*list == NULL)
        *list = new;
    else
    {
        t_redir *cur = *list;
        while (cur->next)
            cur = cur->next;
        cur->next = new;
    }
    return 1;
}

// cleanup helper
void    *cleanup(t_mini *mini, char **full_cmd, t_list **cmds)
{
    if (full_cmd)
        free_all(full_cmd);
    if (mini)
    {
        // free redir list
        t_redir *r = mini->redir;
        while (r)
        {
            t_redir *tmp = r->next;
            free(r->target);
            free(r);
            r = tmp;
        }
        free(mini);
    }
    if (cmds)
        ft_lstclear(cmds, free_mini);
    return NULL;
}

t_list  *get_minis(t_token *tokens)
{
    t_list *cmds = NULL;
    t_mini *mini = init_mini();
    char   **full_cmd = NULL;
    t_list *node;

    if (!mini)
        return NULL;
    while (tokens)
    {
        if (tokens->type == T_WORD)
        {
            full_cmd = get_swords(full_cmd, tokens->value);
            if (!full_cmd)
                return cleanup(mini, full_cmd, &cmds);
        }
        else if (tokens->type == T_REDIR_IN)
        {
            if (!append_redir(&mini->redir, R_IN, tokens->next->value))
                return cleanup(mini, full_cmd, &cmds);
            tokens = tokens->next;
        }
        else if (tokens->type == T_REDIR_OUT)
        {
            if (!append_redir(&mini->redir, R_OUT, tokens->next->value))
                return cleanup(mini, full_cmd, &cmds);
            tokens = tokens->next;
        }
        else if (tokens->type == T_REDIR_APPEND)
        {
            if (!append_redir(&mini->redir, R_APPEND, tokens->next->value))
                return cleanup(mini, full_cmd, &cmds);
            tokens = tokens->next;
        }
        else if (tokens->type == T_HEREDOC)
        {
            if (!append_redir(&mini->redir, R_HEREDOC, tokens->next->value))
                return cleanup(mini, full_cmd, &cmds);
            tokens = tokens->next;
        }
        else if (tokens->type == T_PIPE)
        {
            mini->full_cmd = full_cmd;
            node = ft_lstnew(mini);
            if (!node)
                return cleanup(mini, full_cmd, &cmds);
            ft_lstadd_back(&cmds, node);
            mini = init_mini();
            full_cmd = NULL;
            if (!mini)
                return cleanup(mini, full_cmd, &cmds);
        }
        tokens = tokens->next;
    }
    // add last command
    mini->full_cmd = full_cmd;
    node = ft_lstnew(mini);
    if (!node)
        return cleanup(mini, full_cmd, &cmds);
    ft_lstadd_back(&cmds, node);
    return cmds;
}

