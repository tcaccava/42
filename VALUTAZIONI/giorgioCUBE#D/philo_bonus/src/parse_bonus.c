/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:54:25 by gsabatin          #+#    #+#             */
/*   Updated: 2025/05/22 11:54:43 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <limits.h>

static int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

static int	ft_isspace(int c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

static long	ft_atol(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
		if (i == 11)
			return (0);
	}
	return (result * sign);
}

static int	ft_atol_int(const char *str)
{
	long	num_l;

	num_l = ft_atol(str);
	if (num_l > INT_MAX)
		return (-1);
	else
		return ((int)num_l);
}

int	parse(t_table *table, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (1);
	table->n_philos = ft_atol_int(argv[1]);
	table->time_to_die = ft_atol_int(argv[2]);
	table->time_to_eat = ft_atol_int(argv[3]);
	table->time_to_sleep = ft_atol_int(argv[4]);
	if (argc == 6)
		table->must_eat_times = ft_atol_int(argv[5]);
	else
		table->must_eat_times = NO_MEALS_LIMIT;
	if (table->n_philos < 0
		|| table->time_to_die < 60
		|| table->time_to_eat < 60
		|| table->time_to_eat < 60
		|| table->time_to_sleep < 60
		|| (argc == 6 && table->must_eat_times <= 0))
	{
		ft_putendl_fd("Detected input outside of 60-INT_MAX RANGE.", 2);
		return (1);
	}
	return (0);
}
