/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdonato <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:27:12 by tdonato           #+#    #+#             */
/*   Updated: 2025/02/19 20:07:20 by tdonato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	terminate_program(t_philo *philosophers, t_data *info)
{
	int	i;

	pthread_mutex_destroy(&(info->someone_died_mutex));
	pthread_mutex_destroy(&(info->writing_mutex));
	i = 0;
	if (info->forks)
	{
		while (i < info->philo_nb)
			pthread_mutex_destroy(info->forks + i++);
		free(info->forks);
	}
	i = 0;
	if (philosophers)
	{
		while (i < info->philo_nb)
		{
			pthread_mutex_destroy(&(philosophers[i].eating_mutex));
			pthread_mutex_destroy(&(philosophers[i].times_eaten_mutex));
			i++;
		}
		free(philosophers);
	}
}

int	c_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((int)(time.tv_sec * 1000 + time.tv_usec / 1000));
}

void	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = c_time();
	while ((c_time() - start) < milliseconds)
		usleep(500);
}

int	print_nl(int id, char *action, t_data *info)
{
	pthread_mutex_lock(&(info->someone_died_mutex));
	if (info->someone_died)
	{
		pthread_mutex_unlock(&(info->someone_died_mutex));
		return (1);
	}
	pthread_mutex_lock(&(info->writing_mutex));
	printf("%d %d %s\n", c_time() - info->start, id, action);
	pthread_mutex_unlock(&(info->writing_mutex));
	pthread_mutex_unlock(&(info->someone_died_mutex));
	return (0);
}

int	unlock_forks(t_philo *philosopher, int exit_code)
{
	pthread_mutex_unlock(philosopher->right_fork);
	pthread_mutex_unlock(philosopher->left_fork);
	return (exit_code);
}
