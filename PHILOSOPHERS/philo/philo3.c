/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:55:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/16 12:57:04 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	initialize_philosophers(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->num_of_phil)
	{
		program->philosophers[i].id = i + 1;
		program->philosophers[i].num_of_phil = program->num_of_phil;
		program->philosophers[i].time_to_die = program->time_to_die;
		program->philosophers[i].time_to_eat = program->time_to_eat;
		program->philosophers[i].time_to_sleep = program->time_to_sleep;
		program->philosophers[i].time_must_eat = program->time_must_eat;
		program->philosophers[i].meals_eaten = 0;
		program->philosophers[i].last_meal_time = program->start_time;
		program->philosophers[i].stop_simulation = &program->stop_simulation;
		program->philosophers[i].program = program;
		program->philosophers[i].forks = program->forks;
		pthread_mutex_init(&program->philosophers[i].meal_mutex, NULL);
		i++;
	}
}

void	create_threads(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->num_of_phil)
	{
		pthread_create(&program->threads[i], NULL, philosopher_routine,
			&program->philosophers[i]);
		i++;
	}
	pthread_create(&program->monitor, NULL, monitor_routine, program);
}

void	join_threads(t_program *program)
{
	int	i;

	pthread_join(program->monitor, NULL);
	i = 0;
	while (i < program->num_of_phil)
	{
		pthread_join(program->threads[i], NULL);
		i++;
	}
}

void	cleanup_program(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->num_of_phil)
	{
		pthread_mutex_destroy(&program->philosophers[i].meal_mutex);
		i++;
	}
	i = 0;
	while (i < program->num_of_phil)
	{
		pthread_mutex_destroy(&program->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&program->print_mutex);
	pthread_mutex_destroy(&program->stop_mutex);
	free(program->philosophers);
	free(program->threads);
	free(program->forks);
}

int	get_stop_simulation(t_program *program)
{
	int	stop;

	pthread_mutex_lock(&program->stop_mutex);
	stop = program->stop_simulation;
	pthread_mutex_unlock(&program->stop_mutex);
	return (stop);
}
