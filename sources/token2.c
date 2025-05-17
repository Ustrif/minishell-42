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

void	helpredir(t_token **head, char *s)
{
	ft_tokenadd_back(head, ft_tokennew(s));
}

void	add_redir_token(t_token **head, char *s, int *i, int *last_i)
{
	if (*i > *last_i)
		ft_tokenadd_back(head,
			ft_tokennew(ft_substr(s, *last_i, *i - *last_i)));
	ft_tokenadd_back(head, ft_tokennew(ft_substr(s, *i, 2)));
	*i += 2;
	*last_i = *i;
}

void	add_redir_token1(t_token **head, char *s, int *i, int *last_i)
{
	if (*i > *last_i)
		ft_tokenadd_back(head,
			ft_tokennew(ft_substr(s, *last_i, *i - *last_i)));
	ft_tokenadd_back(head, ft_tokennew(ft_substr(s, *i, 1)));
	(*i)++;
	*last_i = *i;
}

void	update_quote(char c, char *quote)
{
	if (!*quote && (c == '\'' || c == '"'))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

t_token	*split_redirs(char *s, char quote, int i, int last_i)
{
	t_token	*head;

	head = NULL;
	while (s[i])
	{
		update_quote(s[i], &quote);
		if (!quote)
		{
			if ((s[i] == '<' || s[i] == '>') && s[i + 1] == s[i])
			{
				add_redir_token(&head, s, &i, &last_i);
				continue ;
			}
			else if (s[i] == '<' || s[i] == '>')
			{
				add_redir_token1(&head, s, &i, &last_i);
				continue ;
			}
		}
		i++;
	}
	if (i > last_i)
		ft_tokenadd_back(&head, ft_tokennew(ft_substr(s, last_i, i - last_i)));
	return (head);
}
