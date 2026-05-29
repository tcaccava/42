/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:52:48 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 14:52:49 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_philosopher_death_mutex(t_philodata *data, int i)
{
	long long	time_since_last_meal;
	int			should_die;

	pthread_mutex_lock(&data->simulation_mutex);
	time_since_last_meal = get_time_ms() - data->philosophers[i].last_meal_time;
	should_die = time_since_last_meal > data->time_to_die;
	pthread_mutex_unlock(&data->simulation_mutex);
	if (should_die)
	{
		pthread_mutex_lock(&data->simulation_mutex);
		printf("%lld %d died\n", get_time_ms() - data->simulation_start, i + 1);
		data->simulation_running = 0;
		pthread_mutex_unlock(&data->simulation_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_philosophers(void *arg)
{
	t_philodata	*data;
	int			i;

	data = (t_philodata *)arg;
	while (check_simulation_status(data))
	{
		i = 0;
		while (i < data->philo_nb)
		{
			if (check_philosopher_death_mutex(data, i))
				return (NULL);
			i++;
		}
		usleep(900);
	}
	return (NULL);
}

int	begin_simulation(t_philo *philosophers, t_philodata *philo_data)
{
	int	i;

	i = 0;
	philo_data->simulation_start = get_time_ms();
	while (i < philo_data->philo_nb)
	{
		if (pthread_create(&philo_data->threads[i], NULL, philosopher_routine,
				&philosophers[i]) != 0)
		{
			return (0);
		}
		i++;
	}
	if (pthread_create(&philo_data->monitor_thread, NULL,
			(void *(*)(void *))monitor_philosophers, (void *)philo_data) != 0)
	{
		return (0);
	}
	return (1);
}

int	allocate_resources(t_philodata **philo_data, t_philo **philosophers, int ac,
		char **av)
{
	*philo_data = malloc(sizeof(t_philodata));
	if (!*philo_data)
		return (0);
	if (!init_data_philo(*philo_data, ac, av))
	{
		free(*philo_data);
		return (0);
	}
	*philosophers = malloc(sizeof(t_philo) * (*philo_data)->philo_nb);
	if (!*philosophers)
	{
		free((*philo_data)->forks);
		free((*philo_data)->threads);
		free(*philo_data);
		return (0);
	}
	return (1);
}

int	main(int ac, char **av)
{
	int			i;
	t_philodata	*philo_data;
	t_philo		*philosophers;

	i = 1;
	while (i < ac)
	{
		if (!is_valid_number(av[i]))
			return (printf("Error: argument %d is not a valid number\n", i), 1);
		i++;
	}
	if (!allocate_resources(&philo_data, &philosophers, ac, av))
		return (1);
	init_philo(philosophers, philo_data);
	if (!begin_simulation(philosophers, philo_data))
		return (1);
	i = 0;
	while (i < philo_data->philo_nb)
	{
		pthread_join(philo_data->threads[i], NULL);
		i++;
	}
	pthread_join(philo_data->monitor_thread, NULL);
	cleanup_resources(philo_data, philosophers);
	return (0);
}
