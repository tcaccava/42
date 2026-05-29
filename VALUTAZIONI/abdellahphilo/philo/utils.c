/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:52:53 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 14:52:54 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_sleep(long long duration_ms)
{
	long long	start_time;
	long long	current_time;
	long long	elapsed_time;

	start_time = get_time_ms();
	current_time = get_time_ms();
	elapsed_time = current_time - start_time;
	while (1)
	{
		current_time = get_time_ms();
		elapsed_time = current_time - start_time;
		if (elapsed_time > duration_ms)
			break ;
		usleep(95);
	}
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	check_simulation_status(t_philodata *data)
{
	int	status;

	pthread_mutex_lock(&data->simulation_mutex);
	status = data->simulation_running;
	pthread_mutex_unlock(&data->simulation_mutex);
	return (status);
}

void	print_status(t_philodata *data, int id, char *message)
{
	pthread_mutex_lock(&data->simulation_mutex);
	if (data->simulation_running)
	{
		printf("%lld %d %s\n", get_time_ms() - data->simulation_start, id,
			message);
	}
	pthread_mutex_unlock(&data->simulation_mutex);
}

void	determine_forks_order(t_philo *philo, int *first_fork, int *second_fork)
{
	if (philo->id % 2 == 0)
	{
		*first_fork = philo->fork_right;
		*second_fork = philo->fork_left;
	}
	else
	{
		*first_fork = philo->fork_left;
		*second_fork = philo->fork_right;
	}
}
