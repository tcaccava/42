/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:40:53 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 19:23:06 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <semaphore.h>

static void	dining_loop(t_philo *philo)
{
	while (1)
	{
		philo_eat(philo);
		if (philo->table->must_eat_times != NO_MEALS_LIMIT
			&& philo->meals_eaten == philo->table->must_eat_times)
			exit(0);
		philo_sleep(philo);
		philo_think(philo);
		ft_usleep(1);
	}
	return ;
}

static void	*death_monitor_routine(void *arg)
{
	t_philo		*philo;
	long long	time_since_meal;

	philo = (t_philo *)arg;
	philo->last_meal_time = get_time_ms(NULL);
	while (1)
	{
		sem_wait(philo->sem_meal_time);
		time_since_meal = time_diff(philo->last_meal_time, get_time_ms(NULL));
		if (time_since_meal > philo->table->time_to_die)
			print_state(S_DIE, philo);
		sem_post(philo->sem_meal_time);
		ft_usleep(1);
	}
	return (NULL);
}

void	philo_routine(t_table *table, t_philo *philo)
{
	if (launch_thread(&philo->monitor_thread,
			death_monitor_routine, philo) != 0)
		clean_exit(table, "Philo routine launch failed.", 1);
	if (table->n_philos == 1)
		handle_single_philo(philo);
	if (philo->id % 2 == 0)
		ft_usleep(table->time_to_eat);
	dining_loop(philo);
	clean_exit(table, NULL, 1);
}
