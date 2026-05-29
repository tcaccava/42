/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 12:17:11 by girizzi           #+#    #+#             */
/*   Updated: 2026/03/06 10:34:35 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include "philo.h"

int	ft_atoi(char *str)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		if (res > 2147483647)
			return (-1);
		i++;
	}
	return ((int)res);
}

static int	check_isdigit(char *arg)
{
	int	i;

	i = 0;
	if (!arg[i])
		return (1);
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	check_args(char **argv)
{
	if (check_isdigit(argv[1]) || ft_atoi(argv[1]) > PHILO_MAX
		|| ft_atoi(argv[1]) <= 0)
		return (write(2, "Invalid philosophers number\n", 28), 1);
	if (check_isdigit(argv[2]) || ft_atoi(argv[2]) <= 0)
		return (write(2, "Invalid time to die\n", 20), 1);
	if (check_isdigit(argv[3]) || ft_atoi(argv[3]) <= 0)
		return (write(2, "Invalid time to eat\n", 20), 1);
	if (check_isdigit(argv[4]) || ft_atoi(argv[4]) <= 0)
		return (write(2, "Invalid time to sleep\n", 22), 1);
	if (argv[5] && (check_isdigit(argv[5]) || ft_atoi(argv[5]) <= 0))
		return (write(2, "Invalid meals number\n", 21), 1);
	return (0);
}
