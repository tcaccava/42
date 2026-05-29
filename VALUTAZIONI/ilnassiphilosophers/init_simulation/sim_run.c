/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_run.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:08:32 by ilnassi           #+#    #+#             */
/*   Updated: 2026/02/17 15:15:08 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*Stops the simulation by setting the shared sim_running flag to 0.
Protected by sim_mutex to avoid races with other threads.*/
void	stop_sim(t_data *data)
{
	pthread_mutex_lock(&data->sim_mutex);
	data->sim_running = 0;
	pthread_mutex_unlock(&data->sim_mutex);
}

/*Creates one thread per philosopher and one monitor thread.
After all threads are created, initializes the shared start time and
releases the start barrier via set_start_time().
Returns 0 on success, 1 on pthread_create failure.*/
int	start_threads(t_data *data, pthread_t *monitor)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread_id,
				NULL, philo_routine, &data->philos[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(monitor, NULL, monitor_sim, data) != 0)
		return (1);
	set_start_time(data);
	return (0);
}

/*Waits for all philosopher threads and the monitor thread to finish.
Ensures a clean shutdown before freeing resources.*/
void	join_threads(t_data *data, pthread_t monitor)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
		pthread_join(data->philos[i++].thread_id, NULL);
	pthread_join(monitor, NULL);
}
