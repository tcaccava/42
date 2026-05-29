/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:33:54 by lluciani          #+#    #+#             */
/*   Updated: 2025/06/23 15:38:02 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_log(t_philo *philo, char *str)
{
	long	time;
	t_table	*table;

	table = philo->table;
	time = get_current_time() - table->start_time;
	pthread_mutex_lock(&table->mutex.print_log);
	if (is_dead(philo) == 0)
		printf("%ld %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&table->mutex.print_log);
	return ;
}

void	*monitoring_status(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (1)
	{
		if (is_dead(table->philo))
			break ;
		if (philosophers_died(table))
			break ;
		if (all_philo_eat(table))
			break ;
	}
	return (NULL);
}

int	philosophers_died(t_table *table)
{
	long	last_meal;
	int		eating;
	int		i;

	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_lock(&table->mutex.meals_eaten_lock);
		last_meal = table->philo[i].last_meal;
		eating = table->philo[i].eating;
		pthread_mutex_unlock(&table->mutex.meals_eaten_lock);
		if ((get_current_time() - last_meal) >= table->time_to_die
			&& eating == 0)
		{
			print_log(&table->philo[i], "died");
			pthread_mutex_lock(&table->mutex.death_lock);
			table->someone_die = 1;
			pthread_mutex_unlock(&table->mutex.death_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

int	all_philo_eat(t_table *table)
{
	if (check_all_eat(table))
	{
		pthread_mutex_lock(&table->mutex.print_log);
		printf("%ld all philo have eaten enough times!\n", get_current_time()
			- table->start_time);
		pthread_mutex_unlock(&table->mutex.print_log);
		pthread_mutex_lock(&table->mutex.death_lock);
		table->someone_die = 1;
		pthread_mutex_unlock(&table->mutex.death_lock);
		return (1);
	}
	return (0);
}

int	create_thread(t_table *table)
{
	int			i;
	pthread_t	observer;

	i = -1;
	if (pthread_create(&observer, NULL, monitoring_status, table) != 0)
		free_all(table);
	while (++i < table->n_philo)
	{
		if (pthread_create(&table->philo[i].thread, NULL, philo_routine,
				&table->philo[i]) != 0)
		{
			write(2, "error creating Threads\n", 24);
			free_all(table);
			return (0);
		}
	}
	if (pthread_join(observer, NULL) != 0)
		free_all(table);
	i = -1;
	while (++i < table->n_philo)
	{
		if (pthread_join(table->philo[i].thread, NULL) != 0)
			free_all(table);
	}
	return (0);
}
