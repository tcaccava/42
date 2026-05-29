/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:56:59 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 14:27:44 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

bool	is_simulation_ended(t_table *table)
{
	bool	is_ended;

	pthread_mutex_lock(&table->simulation_end_mutex);
	is_ended = table->simulation_end;
	pthread_mutex_unlock(&table->simulation_end_mutex);
	return (is_ended);
}

static int	philo_think(t_philo *philo)
{
	return (print_state(THINK, philo));
}

static int	philo_sleep(t_philo *philo)
{
	int	status;

	status = print_state(SLEEP, philo);
	ft_usleep(philo->table->time_to_sleep);
	return (status);
}

static int	philo_eat(t_philo *philo)
{
	if (print_state(EAT, philo) != 0)
		return (1);
	pthread_mutex_lock(&philo->mutexes[TIME]);
	philo->last_meal_time = get_time_ms(NULL);
	pthread_mutex_unlock(&philo->mutexes[TIME]);
	ft_usleep(philo->table->time_to_eat);
	pthread_mutex_lock(&philo->mutexes[MEALS]);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->mutexes[MEALS]);
	release_forks(philo);
	return (0);
}

void	*philo_routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	if (philo->table->n_philos == 1)
	{
		print_state(FORK, philo);
		ft_usleep(philo->table->time_to_die);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(philo->table->time_to_eat / 2);
	while (1)
	{
		if (take_forks(philo) != 0)
			break ;
		if (philo_eat(philo) != 0)
			break ;
		if (philo_sleep(philo) != 0)
			break ;
		if (philo_think(philo) != 0)
			break ;
	}
	return (NULL);
}
