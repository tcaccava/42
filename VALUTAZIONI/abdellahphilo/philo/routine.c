/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:52:50 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 14:52:51 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Prend les fourchettes et commence à manger
int	take_forks_and_eat(t_philodata *data, t_philo *philo, int first_fork,
		int second_fork)
{
	pthread_mutex_lock(&data->forks[first_fork]);
	if (!check_simulation_status(data))
	{
		pthread_mutex_unlock(&data->forks[first_fork]);
		return (0);
	}
	print_status(data, philo->id, "has taken a fork");
	if (handle_single_philo(data, philo, first_fork))
		return (0);
	pthread_mutex_lock(&data->forks[second_fork]);
	if (!check_simulation_status(data))
	{
		pthread_mutex_unlock(&data->forks[first_fork]);
		pthread_mutex_unlock(&data->forks[second_fork]);
		return (0);
	}
	print_status(data, philo->id, "has taken a fork");
	return (1);
}

// Manger et mettre à jour le compteur de repas
int	eat_and_update(t_philodata *data, t_philo *philo, int first_fork,
		int second_fork)
{
	pthread_mutex_lock(&data->simulation_mutex);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&data->simulation_mutex);
	print_status(data, philo->id, "is eating");
	ft_sleep(data->time_to_eat);
	philo->meals_eaten++;
	if (data->must_eat >= 1)
	{
		if (all_philosophers_eat_enough(data))
		{
			pthread_mutex_lock(&data->simulation_mutex);
			data->simulation_running = 0;
			pthread_mutex_unlock(&data->simulation_mutex);
			pthread_mutex_unlock(&data->forks[second_fork]);
			pthread_mutex_unlock(&data->forks[first_fork]);
			return (0);
		}
	}
	pthread_mutex_unlock(&data->forks[second_fork]);
	pthread_mutex_unlock(&data->forks[first_fork]);
	return (1);
}

// Routine de sommeil et réflexion
int	sleep_and_think(t_philodata *data, t_philo *philo)
{
	if (!check_simulation_status(data))
		return (0);
	print_status(data, philo->id, "is sleeping");
	ft_sleep(data->time_to_sleep);
	if (!check_simulation_status(data))
		return (0);
	print_status(data, philo->id, "is thinking");
	return (1);
}

// Fonction principale pour la routine du philosophe
void	*philosopher_routine(void *arg)
{
	t_philo		*philo;
	t_philodata	*data;
	int			first_fork;
	int			second_fork;

	philo = (t_philo *)arg;
	data = (t_philodata *)philo->philo;
	philo = (t_philo *)arg;
	data = philo->philo;
	determine_forks_order(philo, &first_fork, &second_fork);
	if (philo->id % 2 == 0)
		usleep(1000);
	while (check_simulation_status(data))
	{
		if (!take_forks_and_eat(data, philo, first_fork, second_fork))
			break ;
		if (!eat_and_update(data, philo, first_fork, second_fork))
			break ;
		if (!sleep_and_think(data, philo))
			break ;
	}
	return (NULL);
}
