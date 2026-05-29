/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:52:55 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 14:52:56 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	all_philosophers_eat_enough(t_philodata *data)
{
	int	i;

	i = 0;
	while (i < data->philo_nb)
	{
		if (data->philosophers[i].meals_eaten < data->must_eat)
			return (0);
		i++;
	}
	return (1);
}

// Gère le cas où il n'y a qu'un seul philosophe
int	handle_single_philo(t_philodata *data, t_philo *philo, int first_fork)
{
	if (data->philo_nb == 1)
	{
		while (check_simulation_status(data) && get_time_ms()
			- philo->last_meal_time <= data->time_to_die)
			usleep(100);
		pthread_mutex_unlock(&data->forks[first_fork]);
		return (1);
	}
	return (0);
}

void	cleanup_resources(t_philodata *philo_data, t_philo *philosophers)
{
	if (philosophers)
		free(philosophers);
	if (philo_data)
	{
		if (philo_data->forks)
			free(philo_data->forks);
		if (philo_data->threads)
			free(philo_data->threads);
		free(philo_data);
	}
}
