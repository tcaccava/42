/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:52:45 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 14:52:46 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_philo(t_philo *philosophers, t_philodata *philo_data)
{
	philo_data->i = 0;
	while (philo_data->i < philo_data->philo_nb)
	{
		philosophers[philo_data->i].id = philo_data->i + 1;
		philosophers[philo_data->i].fork_left = philo_data->i;
		philosophers[philo_data->i].fork_right = (philo_data->i + 1)
			% philo_data->philo_nb;
		philosophers[philo_data->i].last_meal_time = get_time_ms();
		philosophers[philo_data->i].meals_eaten = 0;
		philosophers[philo_data->i].philo = philo_data;
		philo_data->i++;
	}
	philo_data->philosophers = philosophers;
	return (1);
}

int	init_data_philo(t_philodata *philo_data, int ac, char **av)
{
	philo_data->philo_nb = ft_atoi(av[1]);
	philo_data->time_to_die = ft_atoi(av[2]);
	philo_data->time_to_eat = ft_atoi(av[3]);
	philo_data->time_to_sleep = ft_atoi(av[4]);
	philo_data->must_eat = -1;
	if (ac == 6)
		philo_data->must_eat = ft_atoi(av[5]);
	philo_data->forks = malloc(sizeof(pthread_mutex_t) * philo_data->philo_nb);
	if (!philo_data->forks)
		return (0);
	philo_data->threads = malloc(sizeof(pthread_t) * philo_data->philo_nb);
	if (!philo_data->threads)
	{
		free(philo_data->forks);
		return (0);
	}
	philo_data->simulation_running = 1;
	pthread_mutex_init(&philo_data->simulation_mutex, NULL);
	philo_data->i = 0;
	while (philo_data->i < philo_data->philo_nb)
	{
		pthread_mutex_init(&philo_data->forks[philo_data->i], NULL);
		philo_data->i++;
	}
	return (1);
}
