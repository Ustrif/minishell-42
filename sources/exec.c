/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raydogmu <raydogmu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:01:38 by codespace         #+#    #+#             */
/*   Updated: 2025/06/28 12:50:00 by raydogmu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_status;

char	**get_fenv(char **e)
{
	char	**result;
	int		i;
	int		env_num;

	result = NULL;
	i = 0;
	env_num = count_words(e);
	while (e[i])
	{
		result = get_swords(result, e[i]);
		i++;
	}
	if (env_num != count_words(result))
	{
		free_all(result);
		return (NULL);
	}
	return (result);
}

t_promp *get_full_promp(char *s, char **env)
{
    t_token	*head;
    t_list  *minis;
    t_promp *p;

	head = get_classified_tokens(s);
	head = get_expanded_tokens(head);
	head = get_unqouted_tokens(head);
	if (!head)
		return (printf("quote err"), NULL);
	minis = get_full_minis(head, env);
	ft_tokenclear(&head);
    if (!minis)
    {
        ft_tokenclear(&head);
        return (printf("s yok"), NULL);
    }
    p = get_prompt(minis, env);
    if (!p)
    {
        ft_tokenclear(&head);
        ft_lstclear(&minis, free_mini);
        return (printf("p"), NULL);
    }
	return (p);
}
