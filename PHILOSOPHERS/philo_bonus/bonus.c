/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 15:47:11 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/24 14:44:28 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		printf("There must be 4 or 5 arguments\n");
		return (0);
	}
	data->num_of_phil = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->times_must_eat = ft_atoi(argv[5]);
	else
		data->times_must_eat = -1;
	if (data->num_of_phil <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0 || (argc == 6
			&& data->times_must_eat <= 0))
	{
		printf("All arguments must be positive integers\n");
		return (0);
	}
	return (1);
}

int	init_data(t_data *data)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/done_eating");
	data->forks = sem_open("/forks", O_CREAT, 0644, data->num_of_phil);
	data->print = sem_open("/print", O_CREAT, 0644, 1);
	data->stop = sem_open("/stop", O_CREAT, 0644, 1);
	data->done_eating = sem_open("/done_eating", O_CREAT, 0644, 0);
	if (data->forks == SEM_FAILED || data->print == SEM_FAILED
		|| data->stop == SEM_FAILED)
		return (printf("Semaphor init failed\n"), 0);
	data->philos = malloc(sizeof(t_philosopher) * data->num_of_phil);
	if (!data->philos)
		return (printf("Error: malloc failed\n"), 0);
	return (1);
}

void	start_simulation(t_data *data)
{
	int			i;
	pthread_t	meal_monitor_thread;

	data->start_time = get_time();
	if (data->times_must_eat != -1)
	{
		if (pthread_create(&meal_monitor_thread, NULL, meal_monitor, data) != 0)
			printf("Error creating meal monitor thread\n");
		pthread_detach(meal_monitor_thread);
	}
	i = 0;
	while (i < data->num_of_phil)
	{
		data->philos[i].id = i + 1;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].data = data;
		data->philos[i].meals_eaten = 0;
		data->philos[i].has_finished = 0;
		data->philos[i].pid = fork();
		if (data->philos[i].pid == 0)
			philosopher_routine(&data->philos[i]);
		i++;
	}
}

void	cleanup(t_data *data)
{
	sem_close(data->forks);
	sem_close(data->stop);
	sem_close(data->print);
	sem_close(data->done_eating);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/done_eating");
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
}

void	philosopher_routine(t_philosopher *phil)
{
	t_data	*data;

	data = phil->data;
	setup_monitor(phil);
	if (phil->id % 2 == 0)
		usleep(data->time_to_eat * 500);
	while (1)
	{
		take_forks_and_eat(phil);
		print_status(phil, "is sleeping");
		usleep(data->time_to_sleep * 1000);
		print_status(phil, "is thinking");
		usleep((data->time_to_die - (data->time_to_sleep + data->time_to_eat))
			/ 2 * (1000));
	}
}
