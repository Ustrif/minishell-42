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

t_token	*split_blanks(char *s, char open, int i, int last_i)
{
	t_token	*head;

	head = NULL;
	while (s[i])
	{
		if (!open && (s[i] == '\'' || s[i] == '"'))
			open = s[i];
		else if (open && s[i] == open)
			open = 0;
		else if (s[i] == ' ' && !open)
		{
			if (i > last_i)
				ft_tokenadd_back(&head, ft_tokennew(
						ft_substr(s, last_i, i - last_i)));
			last_i = i + 1;
		}
		i++;
	}
	if (i > last_i)
		ft_tokenadd_back(&head, ft_tokennew(ft_substr(s, last_i, i - last_i)));
	return (head);
}

t_token	*split_pipes(char *s, char quote, int i, int last_i)
{
	t_token	*head;

	head = NULL;
	while (s[i])
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		else if (s[i] == '|' && !quote)
		{
			if (i > last_i)
				ft_tokenadd_back(&head,
					ft_tokennew(ft_substr(s, last_i, i - last_i)));
			ft_tokenadd_back(&head, ft_tokennew(ft_strdup("|")));
			last_i = i + 1;
		}
		i++;
	}
	if (i > last_i)
		ft_tokenadd_back(&head, ft_tokennew(ft_substr(s, last_i, i - last_i)));
	return (head);
}

t_token	*split_pipes1(char *s)
{
	return (split_pipes(s, 0, 0, 0));
}

t_token	*split_redirs1(char *s)
{
	return (split_redirs(s, 0, 0, 0));
}
