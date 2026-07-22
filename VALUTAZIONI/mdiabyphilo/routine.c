/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiaby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 17:56:57 by mdiaby            #+#    #+#             */
/*   Updated: 2026/07/22 17:56:58 by mdiaby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	get_fork_indexes(t_philo *philo, int *first, int *second)
{
	int	tmp;

	*first = philo->id - 1;
	*second = philo->id % philo->table->number_of_philosophers;
	if (*first > *second)
	{
		tmp = *first;
		*first = *second;
		*second = tmp;
	}
}

static void	take_and_eat(t_table *table, t_philo *philo, int first, int sec)
{
	pthread_mutex_lock(&table->forks[first]);
	ft_print(table, philo->id, "has taken a fork");
	pthread_mutex_lock(&table->forks[sec]);
	ft_print(table, philo->id, "has taken a fork");
	pthread_mutex_lock(&table->lock_meal);
	philo->last_meal = get_time();
	philo->n_time_eats++;
	pthread_mutex_unlock(&table->lock_meal);
	ft_print(table, philo->id, "is eating");
	ft_usleep(table->time_to_eat);
}

static void	*lone_philo(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(&table->forks[0]);
	ft_print(table, philo->id, "has taken a fork");
	ft_usleep(table->time_to_die);
	ft_print(table, philo->id, "died");
	set_dead(table);
	pthread_mutex_unlock(&table->forks[0]);
	return (NULL);
}

static void	rest_and_think(t_table *table, t_philo *philo)
{
	ft_print(table, philo->id, "is sleeping");
	ft_usleep(table->time_to_sleep);
	ft_print(table, philo->id, "is thinking");
	if (table->number_of_philosophers % 2 != 0)
		ft_usleep(table->time_to_eat / 2);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_table	*table;
	int		first;
	int		second;

	philo = (t_philo *)arg;
	table = philo->table;
	if (table->number_of_philosophers == 1)
		return (lone_philo(table, philo));
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!is_dead(table))
	{
		get_fork_indexes(philo, &first, &second);
		take_and_eat(table, philo, first, second);
		pthread_mutex_unlock(&table->forks[first]);
		pthread_mutex_unlock(&table->forks[second]);
		if (must_stop(table, philo))
			break ;
		rest_and_think(table, philo);
	}
	return (NULL);
}
