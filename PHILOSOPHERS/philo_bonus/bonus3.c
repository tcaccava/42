/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:02:32 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/24 14:44:30 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	setup_monitor(t_philosopher *phil)
{
	pthread_t	monitor;

	if (pthread_create(&monitor, NULL, monitor_routine, phil) != 0)
		exit(1);
	pthread_detach(monitor);
}

void	*meal_monitor(void *arg)
{
	t_data			*data;
	int				i;
	t_philosopher	phil;

	data = (t_data *)arg;
	i = 0;
	while (i < data->num_of_phil)
	{
		sem_wait(data->done_eating);
		i++;
	}
	sem_wait(data->print);
	printf("All philosophers have eaten enough meals\n");
	phil.data = data;
	terminate_all_processes(data);
	return (NULL);
}

void	take_forks_and_eat(t_philosopher *phil)
{
	t_data	*data;

	data = phil->data;
	sem_wait(data->forks);
	print_status(phil, "has taken a fork");
	sem_wait(data->forks);
	print_status(phil, "has taken a fork");
	sem_wait(data->stop);
	phil->meals_eaten++;
	phil->last_meal_time = get_time();
	if (data->times_must_eat != -1 && !phil->has_finished
		&& phil->meals_eaten >= data->times_must_eat)
	{
		phil->has_finished = 1;
		sem_post(data->done_eating);
	}
	sem_post(data->stop);
	print_status(phil, "is eating");
	usleep(data->time_to_eat * 1000);
	sem_post(data->forks);
	sem_post(data->forks);
}
