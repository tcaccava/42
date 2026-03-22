/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:22:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/24 15:25:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_philosopher *phil, char *status)
{
	t_data	*data;

	data = phil->data;
	sem_wait(data->print);
	printf("%ld %d %s\n", get_time(), phil->id, status);
	sem_post(data->print);
}

void	terminate_all_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_of_phil)
	{
		if (data->philos[i].pid > 0)
			kill(data->philos[i].pid, SIGKILL);
		i++;
	}
}

void	*monitor_routine(void *arg)
{
	t_philosopher	*phil;
	t_data			*data;

	phil = (t_philosopher *)arg;
	data = phil->data;
	while (1)
	{
		sem_wait(data->stop);
		if ((get_time() - phil->last_meal_time) > data->time_to_die)
		{
			sem_wait(data->print);
			printf("%ld %d died\n", get_time(), phil->id);
			cleanup_and_exit(phil, 1);
		}
		sem_post(data->stop);
		usleep(1000);
	}
	return (NULL);
}

void	cleanup_and_exit(t_philosopher *phil, int exit_code)
{
	t_data	*data;

	data = phil->data;
	sem_close(data->forks);
	sem_close(data->print);
	sem_close(data->stop);
	sem_close(data->done_eating);
	if (data->philos)
		free(data->philos);
	exit(exit_code);
}
