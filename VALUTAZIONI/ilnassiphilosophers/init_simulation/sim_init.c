/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 18:58:01 by ilnassi           #+#    #+#             */
/*   Updated: 2026/02/17 15:08:19 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*Initializes philosopher structures:
- assigns ids and shared data pointer,
- initializes per-philosopher mutexes,
- sets fork pointers (left/right) from the forks array.*/
static int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].food_eaten = 0;
		pthread_mutex_init(&data->philos[i].gnammy_mutex, NULL);
		pthread_mutex_init(&data->philos[i].time_gnammy_mutex, NULL);
		data->philos[i].last_food_time = 0;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		i++;
	}
	return (0);
}

/*Parses arguments, initializes global data, allocates forks/philos arrays,
initializes all mutexes, and prepares philosophers/forks for the simulation.
Returns 0 on success, 1 on error.*/
int	init_all(t_data *data, int argc, char **argv)
{
	int	i;

	if (check_args(argc, argv, data))
		return (1);
	data->sim_running = 1;
	data->someone_died = 0;
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->sim_mutex, NULL);
	data->start_flag = 0;
	pthread_mutex_init(&data->start_mutex, NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->forks || !data->philos)
		return (1);
	i = 0;
	while (i < data->num_philos)
		pthread_mutex_init(&data->forks[i++], NULL);
	return (init_philos(data));
}

/*Destroys all initialized mutexes and frees allocated memory (forks/philos).
Called at the end to clean up resources and avoid leaks.*/
void	destroy_all(t_data *data)
{
	int	i;

	i = 0;
	while (data->philos && i < data->num_philos)
	{
		pthread_mutex_destroy(&data->philos[i].gnammy_mutex);
		pthread_mutex_destroy(&data->philos[i].time_gnammy_mutex);
		i++;
	}
	i = 0;
	while (data->forks && i < data->num_philos)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->sim_mutex);
	pthread_mutex_destroy(&data->start_mutex);
	free(data->forks);
	free(data->philos);
}

/*Start barrier used by philosopher and monitor threads.
Threads spin-wait (with small usleep) until start_flag 
is set by the main thread.*/
void	wait_start(t_data *data)
{
	while (1)
	{
		pthread_mutex_lock(&data->start_mutex);
		if (data->start_flag)
		{
			pthread_mutex_unlock(&data->start_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->start_mutex);
		usleep(100);
	}
}

/*Sets the global start_time and initializes 
each philosopher last_food_time
to the same value, ensuring consistent timing.
Then releases the start barrier by setting start_flag to 1.*/
void	set_start_time(t_data *data)
{
	int	i;

	data->start_time = get_time();
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].time_gnammy_mutex);
		data->philos[i].last_food_time = data->start_time;
		pthread_mutex_unlock(&data->philos[i].time_gnammy_mutex);
		i++;
	}
	pthread_mutex_lock(&data->start_mutex);
	data->start_flag = 1;
	pthread_mutex_unlock(&data->start_mutex);
}
