/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiaby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 17:56:26 by mdiaby            #+#    #+#             */
/*   Updated: 2026/07/22 17:56:29 by mdiaby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_args(int ac)
{
	if (ac < 5 || ac > 6)
	{
		write(2, "Error: invalid arguments\n", 26);
		return (1);
	}
	return (0);
}

int	littlechek_nogood(char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (ft_atoi(av[i]) < 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	int			i;
	long int	res;

	i = 0;
	res = 0;
	if (str == NULL)
		return (-1);
	if (str[0] == '\0')
		return (-1);
	if (str[i] == '-' || str[i] == '+')
		return (-1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		res = res * 10 + (str[i] - '0');
		i++;
		if (res > INT_MAX)
			return (-1);
	}
	return ((int)res);
}

int	check_para(t_table table)
{
	if (table.number_of_philosophers == 0)
		return (1);
	if (table.time_to_die == 0)
		return (1);
	if (table.time_to_eat == 0)
		return (1);
	if (table.time_to_sleep == 0)
		return (1);
	return (0);
}
