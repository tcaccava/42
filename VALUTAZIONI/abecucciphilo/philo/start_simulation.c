/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_eating.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abecucci <abecucci@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 13:01:44 by abecucci          #+#    #+#             */
/*   Updated: 2026/03/02 19:26:20 by abecucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	set_start_time(t_philo *philo)
{
	int		i;
	long	start_time;

	start_time = time_now_ms();
	i = 0;
	philo->sh->start_time = start_time;
	while (i < philo->sh->n)
	{
		philo[i].last_meal = start_time;
		i++;
	}
}

static void	*routine(void *arg)
{
	t_philo	*philos;

	philos = (t_philo *) arg;
	pthread_mutex_lock(&philos->sh->start_cond_mtx);
	pthread_mutex_unlock(&philos->sh->start_cond_mtx);
	philosophers_loop(philos);
	return (NULL);
}

static int	join_philos(t_philo *philo, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
		{
			printf("pthread_join() error\n");
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

static int	create_philos(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->sh->n)
	{
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]) != 0)
		{
			printf("pthread_create() error\n");
			join_philos(philo, i);
			pthread_mutex_unlock(&philo->sh->start_cond_mtx);
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

int	start_simulation(t_philo *philos)
{
	set_start_time(philos);
	pthread_mutex_lock(&philos->sh->start_cond_mtx);
	if (create_philos(philos) != SUCCESS)
		return (FAILURE);
	pthread_mutex_unlock(&philos->sh->start_cond_mtx);
	monitor_loop(philos);
	if (join_philos(philos, philos->sh->n) != SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}
