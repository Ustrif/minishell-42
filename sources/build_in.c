/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beinan <beinan@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:22:38 by beinan            #+#    #+#             */
/*   Updated: 2025/04/30 21:06:59 by beinan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int  get_pwd()
{
    char buffer[1024];

    if (getcwd(buffer, sizeof(buffer)) != NULL)
    {
        //write(1, "dir:  ", 7);
        write(1, &buffer, ft_strlen(buffer));
        return (0);
    }
    else
    {
        //perror("getcwd error");
        return (1);
    }
    
}

//int get_cd()