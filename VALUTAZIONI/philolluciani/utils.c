/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:52:53 by lluciani          #+#    #+#             */
/*   Updated: 2025/06/23 15:52:57 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday error\n", 21);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

size_t	custom_usleep(size_t mls)
{
	size_t	start_time;

	start_time = get_current_time();
	while ((get_current_time() - start_time) < mls)
		usleep(500);
	return (0);
}

void	free_all(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philo)
	{
		pthread_mutex_destroy(&table->mutex.forks[i]);
		i++;
	}
	free(table->philo);
	free(table->mutex.forks);
	pthread_mutex_destroy(&table->mutex.print_log);
	pthread_mutex_destroy(&table->mutex.death_lock);
	pthread_mutex_destroy(&table->mutex.meals_eaten_lock);
}

int	check_all_eat(t_table *table)
{
	int	i;

	i = 0;
	if (table->n_to_eat < 0)
		return (0);
	while (i < table->n_philo)
	{
		pthread_mutex_lock(&table->mutex.meals_eaten_lock);
		if (table->philo[i].meals_eaten < table->n_to_eat)
		{
			pthread_mutex_unlock(&table->mutex.meals_eaten_lock);
			return (0);
		}
		pthread_mutex_unlock(&table->mutex.meals_eaten_lock);
		i++;
	}
	pthread_mutex_lock(&table->mutex.death_lock);
	table->someone_die = 1;
	pthread_mutex_unlock(&table->mutex.death_lock);
	return (1);
}

int	is_dead(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(&table->mutex.death_lock);
	if (table->someone_die == 1)
	{
		pthread_mutex_unlock(&table->mutex.death_lock);
		return (1);
	}
	pthread_mutex_unlock(&table->mutex.death_lock);
	return (0);
}
