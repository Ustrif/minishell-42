// HEADER

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

int single_qoute_num(char *line)
{
    int i;
    int result;
    
    i = 0;
    result = 0;
    if (qoute_num(line, '\'') == -1 || qoute_num(line, '\'') % 2 != 0)
        return (-1);
    else if (qoute_num(line, '\'') == 0)
        return (1);
    while (line[i])
    {

    }
    return (result);
} // tek qoute 'de kaç malloc'luk yer açılacak?
