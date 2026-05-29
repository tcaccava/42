/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdonato <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:53:21 by tdonato           #+#    #+#             */
/*   Updated: 2025/02/19 20:07:20 by tdonato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_philo *philo, t_data *info)
{
	int	dead;

	dead = 0;
	pthread_mutex_lock(&(philo->eating_mutex));
	if (info->time_to_die < c_time() - philo->last_meal_time && !philo->eating)
	{
		dead = 1;
		pthread_mutex_lock(&(info->someone_died_mutex));
		info->someone_died = 1;
		pthread_mutex_unlock(&(info->someone_died_mutex));
		pthread_mutex_lock(&(info->writing_mutex));
		printf("%d %d died\n", c_time() - info->start, philo->id);
		pthread_mutex_unlock(&(info->writing_mutex));
	}
	pthread_mutex_unlock(&(philo->eating_mutex));
	return (dead);
}

int	has_eaten_enough(t_philo *philo, t_data *info, int eaten_enough)
{
	pthread_mutex_lock(&(philo->times_eaten_mutex));
	if (philo->times_eaten < info->number_of_times_each_philosopher_must_eat)
	{
		pthread_mutex_unlock(&(philo->times_eaten_mutex));
		return (0);
	}
	pthread_mutex_unlock(&(philo->times_eaten_mutex));
	return (eaten_enough);
}

void	*observer(t_philo *philos, t_data *info)
{
	int	i;
	int	eaten_enough;

	eaten_enough = 0;
	while (1)
	{
		i = 0;
		if (info->number_of_times_each_philosopher_must_eat > 0)
			eaten_enough = 1;
		while (i < info->philo_nb)
		{
			if (is_dead(philos + i, info))
				return (0);
			eaten_enough = has_eaten_enough(philos + i, info, eaten_enough);
			i++;
		}
		if (eaten_enough)
		{
			pthread_mutex_lock(&(info->someone_died_mutex));
			info->someone_died = 1;
			pthread_mutex_unlock(&(info->someone_died_mutex));
			return (0);
		}
	}
	return (0);
}
