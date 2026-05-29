/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:44:40 by lluciani          #+#    #+#             */
/*   Updated: 2025/06/23 15:44:43 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(&table->mutex.forks[philo->right_fork]);
	print_log(philo, "has taken a fork");
	if (is_one_philo(table) == 0)
		return ;
	pthread_mutex_lock(&table->mutex.forks[philo->left_fork]);
	print_log(philo, "has taken a fork");
	pthread_mutex_lock(&table->mutex.meals_eaten_lock);
	philo->eating = 1;
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&table->mutex.meals_eaten_lock);
	print_log(philo, "is eating");
	custom_usleep(table->time_to_eat);
	pthread_mutex_lock(&table->mutex.meals_eaten_lock);
	philo->eating = 0;
	pthread_mutex_unlock(&table->mutex.meals_eaten_lock);
	pthread_mutex_unlock(&table->mutex.forks[philo->right_fork]);
	pthread_mutex_unlock(&table->mutex.forks[philo->left_fork]);
}

void	philo_think(t_philo *philo)
{
	print_log(philo, "is thinking");
}

void	philo_sleep(t_philo *philo)
{
	print_log(philo, "is sleeping");
	custom_usleep(philo->table->time_to_sleep);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		custom_usleep(100);
	while (!is_dead(philo))
	{
		philo_eat(philo);
		if (is_dead(philo))
			break ;
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

int	is_one_philo(t_table *table)
{
	t_philo	*philo;

	philo = table->philo;
	if (table->n_philo == 1)
	{
		custom_usleep(table->time_to_die);
		pthread_mutex_unlock(&table->mutex.forks[philo->right_fork]);
		print_log(philo, "died");
		pthread_mutex_lock(&table->mutex.death_lock);
		table->someone_die = 1;
		pthread_mutex_unlock(&table->mutex.death_lock);
		return (0);
	}
	return (1);
}
