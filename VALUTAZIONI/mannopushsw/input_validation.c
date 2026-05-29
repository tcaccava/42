/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmanno <fmanno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:29:43 by fmanno            #+#    #+#             */
/*   Updated: 2025/08/26 11:10:38 by fmanno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_isnum(char *str)
{
	int	digits;

	digits = 0;
	if (*str == '+' || *str == '-')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		digits = 1;
		str++;
	}
	return (digits);
}

int	check_duplicate(int n, char **av, int i)
{
	i++;
	while (av[i])
	{
		if (ft_atol(av[i]) == n)
			return (1);
		i++;
	}
	return (0);
}

void	check_args(int ac, char **av)
{
	int		i;
	long	tmp;
	char	**args;

	i = 0;
	if (ac == 2)
		args = ft_split(av[1], ' ');
	else
	{
		i = 1;
		args = av;
	}
	while (args[i])
	{
		if (!ft_isnum(args[i]))
			handle_error("Error");
		tmp = ft_atol(args[i]);
		if (tmp < -2147483648 || tmp > 2147483647)
			handle_error("Error");
		if (check_duplicate((int)tmp, args, i))
			handle_error("Error");
		i++;
	}
	if (ac == 2)
		free_array(args);
}
