/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:28:17 by rodolhop          #+#    #+#             */
/*   Updated: 2025/06/30 12:28:18 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_if_dead(t_data *data)
{
	int			i;
	long long	time_since_last_meal;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_mutex);
		time_since_last_meal = get_time() - data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->meal_mutex);
		if (time_since_last_meal > data->time_to_die)
		{
			pthread_mutex_lock(&data->death_mutex);
			data->someone_died = true;
			pthread_mutex_unlock(&data->death_mutex);
			pthread_mutex_lock(&data->write_mutex);
			printf("%lld %d died\n", get_time() - data->start_time,
				data->philos[i].id);
			pthread_mutex_unlock(&data->write_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_if_all_ate(t_data *data)
{
	int	i;
	int	count;

	if (data->meals_limit == -1)
		return (0);
	count = 0;
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_mutex);
		if (data->philos[i].meals_eaten >= data->meals_limit)
			count++;
		pthread_mutex_unlock(&data->meal_mutex);
		i++;
	}
	if (count == data->num_philos)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (!data->all_ate_enough)
			data->all_ate_enough = true;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_if_dead(data))
			break ;
		if (check_if_all_ate(data))
			break ;
		usleep(100);
	}
	return (NULL);
}
