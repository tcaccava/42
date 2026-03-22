/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:55:24 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/16 13:19:13 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_philosopher *phil, char *status)
{
	pthread_mutex_t	*print_mutex;

	print_mutex = &phil->program->print_mutex;
	pthread_mutex_lock(print_mutex);
	if (!get_stop_simulation(phil->program))
		printf("%ld %d %s\n", get_time(), phil->id, status);
	pthread_mutex_unlock(print_mutex);
}

void	take_forks(t_philosopher *phil)
{
	if (get_stop_simulation(phil->program))
		return ;
	if (phil->num_of_phil == 1)
	{
		pthread_mutex_lock(&phil->forks[0]);
		print_status(phil, "has taken a fork");
		while (!get_stop_simulation(phil->program))
			usleep(1000);
		pthread_mutex_unlock(&phil->forks[0]);
		return ;
	}
	if (phil->id % 2 == 0)
	{
		pthread_mutex_lock(&phil->forks[phil->id - 1]);
		print_status(phil, "has taken a left fork");
		pthread_mutex_lock(&phil->forks[phil->id % phil->num_of_phil]);
		print_status(phil, "has taken a right fork");
	}
	else
	{
		pthread_mutex_lock(&phil->forks[phil->id % phil->num_of_phil]);
		print_status(phil, "has taken a right fork");
		pthread_mutex_lock(&phil->forks[phil->id - 1]);
		print_status(phil, "has taken a left fork");
	}
}

void	release_forks(t_philosopher *phil)
{
	pthread_mutex_unlock(&phil->forks[phil->id - 1]);
	pthread_mutex_unlock(&phil->forks[phil->id % phil->num_of_phil]);
}

void	eat_sleep_think(t_philosopher *phil, int time_to_think)
{
	if (get_stop_simulation(phil->program))
		return ;
	take_forks(phil);
	if (get_stop_simulation(phil->program))
	{
		release_forks(phil);
		return ;
	}
	pthread_mutex_lock(&phil->meal_mutex);
	phil->last_meal_time = get_time();
	pthread_mutex_unlock(&phil->meal_mutex);
	print_status(phil, "is eating");
	usleep(phil->time_to_eat * 1000);
	pthread_mutex_lock(&phil->meal_mutex);
	phil->meals_eaten++;
	pthread_mutex_unlock(&phil->meal_mutex);
	release_forks(phil);
	print_status(phil, "is sleeping");
	usleep(phil->time_to_sleep * 1000);
	print_status(phil, "is thinking");
	if (time_to_think > 0)
		usleep(time_to_think * 1000);
}
