/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:53:25 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 15:16:35 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	check_philosopher_death(t_philodata *data, int i)
{
	long long	time_since_last_meal;
	int			should_die;

	sem_wait(data->status_sem);
	time_since_last_meal = get_time_ms() - data->philosophers[i].last_meal_time;
	should_die = time_since_last_meal > data->time_to_die;
	sem_post(data->status_sem);
	if (should_die)
	{
		sem_wait(data->status_sem);
		printf("%lld %d died\n", get_time_ms() - data->start_time, i + 1);
		data->simulation_running = 0;
		sem_post(data->status_sem);
		return (1);
	}
	return (0);
}

void	*monitor_philosophers_bonus(void *arg)
{
	int			i;
	t_philodata	*data;

	i = 0;
	data = (t_philodata *)arg;
	while (check_simulation_status_bonus(data))
	{
		data->i = 0;
		while (data->i < data->philo_nb)
		{
			if (check_philosopher_death(data, data->i))
			{
				while (i < 0)
				{
					if (data->philosophers[i].pid > 0)
						kill(data->philosophers[i].pid, SIGTERM);
					i++;
				}
				return (NULL);
			}
			data->i++;
		}
		usleep(900);
	}
	return (NULL);
}

int	begin_simulation_bonus(t_philo *philosophers, t_philodata *philo_data)
{
	int	i;

	(void)philosophers;
	i = 0;
	while (i < philo_data->philo_nb)
	{
		philo_data->philosophers[i].pid = fork();
		if (philo_data->philosophers[i].pid == 0)
		{
			philosopher_routine_bonus(&philo_data->philosophers[i]);
			exit(0);
		}
		if (philo_data->philosophers[i].pid < 0)
			return (1);
		i++;
	}
	monitor_philosophers_bonus(philo_data);
	i = 0;
	while (i < philo_data->philo_nb)
	{
		waitpid(philo_data->philosophers[i].pid, NULL, 0);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_philodata	*philo_data;
	int			result;
	int			i;

	i = 1;
	while (i < ac)
	{
		if (!is_valid_number(av[i]))
			return (printf("Error: argument %d is not a valid number\n", i), 1);
		i++;
	}
	if (ac < 5 || ac > 6)
		return (printf("Error: wrong number of arguments\n"), 1);
	philo_data = malloc(sizeof(t_philodata));
	if (!philo_data)
		return (1);
	if (!init_data_philo_bonus(ac, av, philo_data))
	{
		free(philo_data);
		return (1);
	}
	init_philo_bonus(philo_data, philo_data->philosophers);
	result = begin_simulation_bonus(philo_data->philosophers, philo_data);
	cleanup_resources(philo_data);
	return (result);
}
