#include "minishell.h"

int	ft_is_numeric(const char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	ft_exit(char **args)
{
	int exit_status;

	write(1, "exit\n", 5);
	if (!args[1])
		exit(0);
	if (!ft_is_numeric(args[1]))
	{
		write(2, "minishell: exit: numeric argument required\n", 43);
		exit(255);
	}
	if (args[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	exit_status = ft_atoi(args[1]) % 256;
	if (exit_status < 0)
		exit_status += 256;
	exit(exit_status);
}
