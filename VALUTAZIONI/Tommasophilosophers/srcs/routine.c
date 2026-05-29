/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdonato <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:53:21 by tdonato           #+#    #+#             */
/*   Updated: 2025/02/18 13:53:21 by tdonato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	routine_eat(t_philo *philo, t_data *info)
{
	pthread_mutex_lock(philo->right_fork);
	if (print_nl(philo->id, "has taken a fork", info) || info->philo_nb == 1)
	{
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&(philo->eating_mutex));
	philo->eating = 1;
	philo->last_meal_time = c_time();
	pthread_mutex_unlock(&(philo->eating_mutex));
	if (print_nl(philo->id, "is eating", info))
		return (unlock_forks(philo, 1));
	pthread_mutex_lock(&(philo->times_eaten_mutex));
	philo->times_eaten += 1;
	pthread_mutex_unlock(&(philo->times_eaten_mutex));
	ft_usleep(info->time_to_eat);
	pthread_mutex_lock(&(philo->eating_mutex));
	philo->eating = 0;
	pthread_mutex_unlock(&(philo->eating_mutex));
	return (unlock_forks(philo, 0));
}

int	routine_sleep(t_philo *philosopher, t_data *info)
{
	if (print_nl(philosopher->id, "is sleeping", info))
		return (1);
	ft_usleep(info->time_to_sleep);
	return (0);
}

int	routine_think(t_philo *philosopher, t_data *info)
{
	if (print_nl(philosopher->id, "is thinking", info))
		return (1);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philosopher;
	t_data	*info;

	philosopher = (t_philo *)arg;
	info = philosopher->info;
	if (philosopher->id % 2)
	{
		if (routine_sleep(philosopher, info))
			return (0);
		if (routine_think(philosopher, info))
			return (0);
	}
	while (1)
	{
		if (routine_eat(philosopher, info))
			return (0);
		if (routine_sleep(philosopher, info))
			return (0);
		if (routine_think(philosopher, info))
			return (0);
	}
	return (0);
}
