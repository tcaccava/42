/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 16:01:09 by ilnassi           #+#    #+#             */
/*   Updated: 2026/02/25 19:17:51 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*Checks if a single philosopher has reached the required number of meals.
Returns 0 if must_eat is not set. 
Access to food_eaten is protected by a mutex.*/
static int	philo_is_full(t_philo *philo)
{
	int	full;

	if (philo->data->must_eat < 0)
		return (0);
	pthread_mutex_lock(&philo->gnammy_mutex);
	full = (philo->food_eaten >= philo->data->must_eat);
	pthread_mutex_unlock(&philo->gnammy_mutex);
	return (full);
}

/*Returns 1 if all philosophers have eaten at least must_eat times.
Used by the monitor to stop the simulation when the goal is reached.*/
static int	philos_are_full(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (!philo_is_full(&data->philos[i]))
			return (0);
		i++;
	}
	return (1);
}

/*Iterates over all philosophers and calls check_death().
Returns 1 as soon as a death 
is detected (or simulation already stopped).*/
static int	check_all_deaths(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (check_death(&data->philos[i]))
			return (1);
		i++;
	}
	return (0);
}

/*Decides if the simulation must stop:
- stops immediately on death detection,
- stops if must_eat is set and all philosophers are full.
Returns 1 when the monitor should exit, 0 otherwise.*/
static int	sim_stop(t_data *data)
{
	if (check_all_deaths(data))
		return (1);
	if (data->must_eat >= 0 && philos_are_full(data))
	{
		stop_sim(data);
		return (1);
	}
	return (0);
}

/*Dedicated monitor thread routine.
Waits for the global start barrier, then periodically checks for:
- philosopher death (time_to_die),
- optional completion condition (must_eat).
Stops the simulation when needed.*/
void	*monitor_sim(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	wait_start(data);
	while (sim_is_running(data))
	{
		if (sim_stop(data))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
