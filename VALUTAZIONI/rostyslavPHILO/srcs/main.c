/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:27:13 by rodolhop          #+#    #+#             */
/*   Updated: 2025/06/30 12:27:22 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_time();
	while (i < data->num_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		if (pthread_create(&data->philos[i].thread, NULL,
				philo_routine, &data->philos[i]))
			return (ft_error("Failed to create thread"));
		i++;
	}
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data))
		return (ft_error("Failed to create monitor thread"));
	return (SUCCESS);
}

int	join_threads(t_data *data)
{
	int	i;

	pthread_join(data->monitor_thread, NULL);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (check_args(argc, argv) != SUCCESS)
		return (ERROR);
	if (init_data(&data, argc, argv) != SUCCESS)
		return (ERROR);
	if (init_mutexes(&data) != SUCCESS)
	{
		free_all(&data);
		return (ERROR);
	}
	if (init_philos(&data) != SUCCESS)
	{
		free_all(&data);
		return (ERROR);
	}
	if (create_threads(&data) != SUCCESS)
	{
		free_all(&data);
		return (ERROR);
	}
	join_threads(&data);
	free_all(&data);
	return (SUCCESS);
}
