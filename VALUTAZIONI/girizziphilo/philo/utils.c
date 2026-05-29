/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 10:40:00 by girizzi           #+#    #+#             */
/*   Updated: 2026/03/06 12:57:16 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long long time, t_program *program)
{
	long long	start;

	start = get_time();
	while (1)
	{
		pthread_mutex_lock(&program->dead_lock);
		if (program->dead_flag)
		{
			pthread_mutex_unlock(&program->dead_lock);
			break ;
		}
		pthread_mutex_unlock(&program->dead_lock);
		if (get_time() - start >= time)
			break ;
		usleep(500);
	}
}

void	print_status(t_philo *philo, char *status)
{
	long long	time;

	pthread_mutex_lock(&philo->program->dead_lock);
	if (philo->program->dead_flag)
	{
		pthread_mutex_unlock(&philo->program->dead_lock);
		return ;
	}
	pthread_mutex_unlock(&philo->program->dead_lock);
	pthread_mutex_lock(&philo->program->write_lock);
	time = get_time() - philo->program->start_time;
	printf("%lld %d %s\n", time, philo->id, status);
	pthread_mutex_unlock(&philo->program->write_lock);
}
