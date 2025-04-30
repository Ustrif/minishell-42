/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 19:59:53 by raydogmu          #+#    #+#             */
/*   Updated: 2025/04/30 21:07:21 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int qoute_num(char *line, char type)
{
    int result;
    int i;

    result = 0;
    i = 0;
    if (!line)
        return (-1);
    while (line[i])
    {
        if (line[i] == type)
            result++;
        i++;
    }
    return (result);
}

int is_quite(char c)
{
    if (c == '\'' || c == '"')
        return (1);
    return (0);
}

t_list  *get_token(char *line, int a, int b)
{
    t_list  *head;
    char    *content;
    int     i;

    if (!line || b < a)
        return (NULL);
    content = malloc(b - a + 1);
    i = 0;
    if (!content)
        return (NULL);
    while (i < (b - a))
    {
        content[i] = line[a + i];
        i++;
    }
    content[b - a] = '\0';
    head = ft_lstnew(content);
    if (!head)
    {
        free(content);
        return (NULL);
    }
    return (head);
}

t_list  *get_tokens(char *line)
{
    int     last_i;
    int     i;
    t_list  *head;

    i = 0;
    last_i = 0;
    while (line[i])
    {
        if (is_quite(line[i]))
            
        i++;
    }
}

int main(void)
{
    t_list *head;

    head = get_token("merhaba deneme", 3, 7);
    printf("cikti: %s\n", (char *) head->content);
    free(head->content);
    free(head);
    return (0);
} // deneme amaçlı.
