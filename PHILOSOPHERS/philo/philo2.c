/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 12:53:45 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/24 15:33:35 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher	*phil;
	int				time_to_think;
	t_program		*program;

	phil = (t_philosopher *)arg;
	program = phil->program;
	time_to_think = (phil->time_to_die - (phil->time_to_eat
				+ phil->time_to_sleep)) / 2;
	if (time_to_think < 0)
		time_to_think = 0;
	if (phil->id % 2 == 0)
		usleep(1000);
	while (!get_stop_simulation(program))
		eat_sleep_think(phil, time_to_think);
	return (NULL);
}

void	check_death_full(t_philosopher *phil, t_program *program)
{
	long	current_time;
	long	time_since_last_meal;

	pthread_mutex_lock(&phil->meal_mutex);
	current_time = get_time();
	time_since_last_meal = current_time - phil->last_meal_time;
	if (time_since_last_meal > (long)phil->time_to_die)
	{
		set_stop_simulation(program, 1);
		pthread_mutex_lock(&program->print_mutex);
		printf("%ld %d died\n", get_time(), phil->id);
		pthread_mutex_unlock(&program->print_mutex);
		pthread_mutex_unlock(&phil->meal_mutex);
		return ;
	}
	if (phil->time_must_eat != -1 && phil->meals_eaten < phil->time_must_eat)
		phil->program->all_finished = 0;
	pthread_mutex_unlock(&phil->meal_mutex);
}

void	*monitor_routine(void *arg)
{
	t_program		*program;
	t_philosopher	*philosophers;
	int				i;

	program = (t_program *)arg;
	philosophers = program->philosophers;
	while (!get_stop_simulation(program))
	{
		program->all_finished = 1;
		i = 0;
		while (i < program->num_of_phil && !get_stop_simulation(program))
		{
			check_death_full(&philosophers[i], program);
			i++;
		}
		check_all_fed(program);
		usleep(1000);
	}
	return (NULL);
}

int	parse_arguments(int argc, char **argv, t_program *program)
{
	if (argc < 5 || argc > 6)
	{
		printf("There must be 4 or 5 arguments\n");
		return (0);
	}
	program->num_of_phil = ft_atoi(argv[1]);
	program->time_to_die = ft_atoi(argv[2]);
	program->time_to_eat = ft_atoi(argv[3]);
	program->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		program->time_must_eat = ft_atoi(argv[5]);
	else
		program->time_must_eat = -1;
	if (program->num_of_phil <= 0 || program->time_to_die <= 0
		|| program->time_to_eat <= 0 || program->time_to_sleep <= 0
		|| (argc == 6 && program->time_must_eat <= 0))
	{
		printf("All arguments must be positive integers\n");
		return (0);
	}
	return (1);
}

int	initialize_program(t_program *program)
{
	int	i;

	program->philosophers = malloc(sizeof(t_philosopher)
			* program->num_of_phil);
	program->threads = malloc(sizeof(pthread_t) * program->num_of_phil);
	program->forks = malloc(sizeof(pthread_mutex_t) * (program->num_of_phil));
	if (!program->philosophers || !program->threads || !program->forks)
	{
		printf("Memory allocation error\n");
		return (0);
	}
	program->stop_simulation = 0;
	program->start_time = get_time();
	pthread_mutex_init(&program->stop_mutex, NULL);
	pthread_mutex_init(&program->print_mutex, NULL);
	i = 0;
	while (i < program->num_of_phil)
	{
		pthread_mutex_init(&program->forks[i], NULL);
		i++;
	}
	return (1);
}
