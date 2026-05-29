/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:42:25 by girizzi           #+#    #+#             */
/*   Updated: 2026/03/06 10:34:56 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_program(t_program *program, char **argv)
{
	program->n_philos = ft_atoi(argv[1]);
	program->t_die = ft_atoi(argv[2]);
	program->t_eat = ft_atoi(argv[3]);
	program->t_sleep = ft_atoi(argv[4]);
	program->must_eat = -1;
	if (argv[5])
		program->must_eat = ft_atoi(argv[5]);
	program->dead_flag = 0;
	program->start_time = get_time();
	if (pthread_mutex_init(&program->write_lock, NULL))
		return (1);
	if (pthread_mutex_init(&program->dead_lock, NULL))
		return (1);
	if (pthread_mutex_init(&program->meal_lock, NULL))
		return (1);
	return (0);
}

int	init_forks(t_program *program)
{
	int	i;

	program->forks = malloc(sizeof(pthread_mutex_t) * program->n_philos);
	if (!program->forks)
		return (1);
	i = 0;
	while (i < program->n_philos)
	{
		if (pthread_mutex_init(&program->forks[i], NULL))
			return (1);
		i++;
	}
	return (0);
}

void	init_philos(t_program *program)
{
	int	i;

	program->philos = malloc(sizeof(t_philo) * program->n_philos);
	if (!program->philos)
		return ;
	i = 0;
	while (i < program->n_philos)
	{
		program->philos[i].id = i + 1;
		program->philos[i].meals_eaten = 0;
		program->philos[i].last_meal = program->start_time;
		program->philos[i].program = program;
		program->philos[i].left_fork = &program->forks[i];
		program->philos[i].right_fork = &program->forks[(i + 1)
			% program->n_philos];
		i++;
	}
}
