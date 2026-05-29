/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:23:44 by lluciani          #+#    #+#             */
/*   Updated: 2025/06/23 15:23:49 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_numstr(char **s)
{
	int	i;

	i = 1;
	while (s[i] != NULL)
	{
		i++;
	}
	return (i);
}

static int	is_number(char c)
{
	return (c >= '0' && c <= '9');
}

static int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

long	ft_atol(char *s)
{
	int		i;
	long	res;
	int		sign;

	i = 0;
	res = 0;
	sign = 1;
	while (is_space(s[i]))
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			return (-1);
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = res * 10 + (s[i] - '0');
		if (res > INT_MAX)
			return (-1);
		i++;
	}
	return (res * sign);
}

int	check_input(char **av)
{
	int		i;
	int		j;
	int		size;
	long	val;

	i = 1;
	j = 0;
	size = ft_numstr(av);
	if (!*av || av[0][0] == '\0')
		return (0);
	val = ft_atol(av[1]);
	if (val == -1)
		return (0);
	while (i < size)
	{
		j = 0;
		while (av[i][j])
		{
			if (!is_number(av[i][j]))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
