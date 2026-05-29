/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:27:53 by rodolhop          #+#    #+#             */
/*   Updated: 2025/06/30 12:27:54 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, int argc, char **argv)
{
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->meals_limit = -1;
	if (argc == 6)
		data->meals_limit = ft_atoi(argv[5]);
	data->someone_died = false;
	data->all_ate_enough = false;
	data->philos = NULL;
	data->forks = NULL;
	return (SUCCESS);
}

int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (ft_error("Malloc failed for forks"));
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (ft_error("Mutex init failed for fork"));
		i++;
	}
	if (pthread_mutex_init(&data->write_mutex, NULL))
		return (ft_error("Mutex init failed for write"));
	if (pthread_mutex_init(&data->death_mutex, NULL))
		return (ft_error("Mutex init failed for death"));
	if (pthread_mutex_init(&data->meal_mutex, NULL))
		return (ft_error("Mutex init failed for meal"));
	return (SUCCESS);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (ft_error("Malloc failed for philosophers"));
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].left_fork = i;
		data->philos[i].right_fork = (i + 1) % data->num_philos;
		data->philos[i].last_meal_time = 0;
		data->philos[i].data = data;
		i++;
	}
	return (SUCCESS);
}
