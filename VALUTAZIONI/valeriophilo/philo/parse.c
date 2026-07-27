/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 10:02:24 by valucare          #+#    #+#             */
/*   Updated: 2026/07/27 17:18:11 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_unsigned_number(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

long	parse_long(const char *str)
{
	long	value;
	int		i;

	value = 0;
	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		value = value * 10 + (str[i] - '0');
		if (value > INT_CEIL)
			return (-1);
		i++;
	}
	return (value);
}

int	check_args(int argc, char **argv)
{
	int		i;
	long	value;

	i = 1;
	while (i < argc)
	{
		if (!is_unsigned_number(argv[i]))
			return (printf("%s", ERR_ARGV), 1);
		value = parse_long(argv[i]);
		if (value < 0 || (i < 5 && value < 1))
			return (printf("%s", ERR_ARGV), 1);
		i++;
	}
	return (0);
}
