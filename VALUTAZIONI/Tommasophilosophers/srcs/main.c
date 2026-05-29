/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdonato <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:27:12 by tdonato           #+#    #+#             */
/*   Updated: 2025/02/19 20:07:20 by tdonato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*create_philosophers(t_data *info)
{
	int		i;
	t_philo	*philosophers;

	i = 0;
	philosophers = malloc(info->philo_nb * sizeof(t_philo));
	if (!info->forks)
	{
		terminate_program(philosophers, info);
		return (0);
	}
	while (i < info->philo_nb)
	{
		philosophers[i].id = i;
		philosophers[i].info = info;
		philosophers[i].eating = 0;
		philosophers[i].last_meal_time = c_time();
		philosophers[i].times_eaten = 0;
		pthread_mutex_init(&(philosophers[i].eating_mutex), 0);
		pthread_mutex_init(&(philosophers[i].times_eaten_mutex), 0);
		philosophers[i].right_fork = info->forks + i;
		philosophers[i].left_fork = &(info->forks[(i + 1) % info->philo_nb]);
		i++;
	}
	return (philosophers);
}

void	run_threads(t_philo *philo, t_data *info)
{
	int	i;

	i = 0;
	info->start = c_time();
	while (i < info->philo_nb)
	{
		pthread_create(&(philo[i].thread), NULL, routine, philo + i);
		i++;
	}
}

void	join_threads(t_philo *philo, t_data *info)
{
	int	i;

	i = 0;
	while (i < info->philo_nb)
	{
		pthread_join(philo[i].thread, 0);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data	info;
	t_philo	*philosophers;
	int		inp;

	if (argc != 5 && argc != 6)
	{
		wrong_args();
		return (1);
	}
	inp = get_input(argc, argv, &info, 1);
	if (inp != -1)
		return (inp);
	philosophers = create_philosophers(&info);
	if (!philosophers)
		return (1);
	run_threads(philosophers, &info);
	observer(philosophers, &info);
	join_threads(philosophers, &info);
	terminate_program(philosophers, &info);
	return (0);
}
