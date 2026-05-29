/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:09:51 by rodolhop          #+#    #+#             */
/*   Updated: 2025/06/25 16:09:53 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		print_status(philo->data, philo->id, "has taken a fork");
		if (philo->data->num_philos == 1)
		{
			ft_sleep(philo->data->time_to_die + 1);
			pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
			return ;
		}
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		print_status(philo->data, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		print_status(philo->data, philo->id, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		print_status(philo->data, philo->id, "has taken a fork");
	}
}

void	eat(t_philo *philo)
{
	print_status(philo->data, philo->id, "is eating");
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	ft_sleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
}

void	sleep_and_think(t_philo *philo)
{
	print_status(philo->data, philo->id, "is sleeping");
	ft_sleep(philo->data->time_to_sleep);
	print_status(philo->data, philo->id, "is thinking");
	if (philo->data->num_philos % 2 == 1)
	{
		usleep(500);
	}
}

int	check_death_condition(t_philo *philo)
{
	int	should_stop;

	pthread_mutex_lock(&philo->data->death_mutex);
	should_stop = philo->data->someone_died || philo->data->all_ate_enough;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (should_stop);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(150);
	while (!check_death_condition(philo))
	{
		take_forks(philo);
		if (philo->data->num_philos == 1)
			break ;
		eat(philo);
		if (check_death_condition(philo))
			break ;
		sleep_and_think(philo);
	}
	return (NULL);
}
