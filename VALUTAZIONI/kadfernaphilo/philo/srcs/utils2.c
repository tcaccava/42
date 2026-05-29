/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadferna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:22:57 by kadferna          #+#    #+#             */
/*   Updated: 2025/06/12 12:23:32 by kadferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

bool	if_odd(int n)
{
	if (n % 2 == 0)
		return (false);
	else
		return (true);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (!philo->data->death_flag)
	{
		pthread_mutex_lock(&philo->data->write_mutex);
		printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id,
			status);
		pthread_mutex_unlock(&philo->data->write_mutex);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
}

int	pick_forks_and_eat(t_philo *philo, pthread_mutex_t *first_fork,
		pthread_mutex_t *second_fork)
{
	if (philo->id % 2 != 0)
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");
	if (philo->data->num_philos == 1)
	{
		usleep(philo->data->time_to_die * 1000);
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
	return (1);
}

int	check_death_flag(t_data *data, t_philo *philos, long time_since_last_meal,
		int i)
{
	if (time_since_last_meal > data->time_to_die)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (!data->death_flag)
		{
			data->death_flag = true;
			pthread_mutex_lock(&data->write_mutex);
			printf("%ld %d died\n", get_time() - data->start_time,
				philos[i].id);
			pthread_mutex_unlock(&data->write_mutex);
		}
		pthread_mutex_unlock(&data->death_mutex);
		return (0);
	}
	return (1);
}
