/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadferna <marvin.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 13:53:05 by kadferna          #+#    #+#             */
/*   Updated: 2025/05/14 13:53:06 by kadferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

static void	initialize_philo_properties(t_philo *philo, t_data *data, int id)
{
	philo->id = id;
	philo->meals_eaten = 0;
	philo->last_meal_time = get_time();
	philo->data = data;
	philo->left_fork = NULL;
	philo->right_fork = NULL;
}

t_data	*init_data(int *nums)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->num_philos = nums[0];
	data->time_to_die = nums[1];
	data->time_to_eat = nums[2];
	data->time_to_sleep = nums[3];
	data->max_meals = nums[4];
	data->forks = NULL;
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->death_mutex);
		return (free(data), NULL);
	}
	if (pthread_mutex_init(&data->meal_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->write_mutex);
		pthread_mutex_destroy(&data->death_mutex);
		return (free(data), NULL);
	}
	return (data);
}

t_philo	*init_philos(t_data *data)
{
	t_philo	*philos;
	int		i;

	if (!data)
		return (NULL);
	philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < data->num_philos)
	{
		initialize_philo_properties(&philos[i], data, i + 1);
		i++;
	}
	return (philos);
}

int	init_forks(t_data *data, t_philo *philos)
{
	int	i;

	i = -1;
	data->death_flag = false;
	data->start_time = get_time();
	while (++i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			return (0);
		}
	}
	i = -1;
	while (++i < data->num_philos)
	{
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
	}
	return (1);
}

t_data	*init(int *nums, t_philo **philos)
{
	t_data	*data;

	data = init_data(nums);
	if (!data)
		return (std_error("Initializing data failed"), NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
	{
		pthread_mutex_destroy(&data->death_mutex);
		pthread_mutex_destroy(&data->write_mutex);
		free(data);
		return (std_error("could not allocate memory for Forks"), NULL);
	}
	*philos = init_philos(data);
	if (!*philos)
		return (free_data_struct(data, NULL), NULL);
	if (!init_forks(data, *philos))
	{
		free_philos(*philos);
		destroy_mutex(data);
		return (std_error("Initializing forks failed"), NULL);
	}
	return (data);
}
