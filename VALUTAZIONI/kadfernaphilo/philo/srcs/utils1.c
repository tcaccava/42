/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadferna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 13:32:51 by kadferna          #+#    #+#             */
/*   Updated: 2025/05/16 13:32:55 by kadferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

int	check_numbers(int *nums, int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
			return (std_error("Must be in range of INT_MIN and INT_MAX"), 0);
		nums[i - 1] = ft_atoi(argv[i]);
		i++;
	}
	if (argc == 6)
	{
		nums[4] = ft_atoi(argv[5]);
		if (nums[4] < 0)
			return (0);
	}
	else if (argc == 5)
		nums[4] = -1;
	return (1);
}

void	ft_usleep(time_t milliseconds, t_data *data)
{
	time_t	start;

	start = get_time();
	while (get_time() - start < milliseconds)
	{
		usleep(100);
		if (data->death_flag)
			return ;
	}
}

time_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
