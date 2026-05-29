/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:53:23 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 14:53:24 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	init_philo_bonus(t_philodata *philo_data, t_philo *philosophers)
{
	int	i;

	(void)philosophers;
	i = 0;
	while (i < philo_data->philo_nb)
	{
		philo_data->philosophers[i].id = i + 1;
		philo_data->philosophers[i].fork_left = i;
		philo_data->philosophers[i].fork_right = (i + 1) % philo_data->philo_nb;
		philo_data->philosophers[i].pid = 0;
		philo_data->philosophers[i].last_meal_time = 0;
		philo_data->philosophers[i].meals_eaten = 0;
		philo_data->philosophers[i].philo_data = philo_data;
		i++;
	}
}

int	init_fork_semaphores(t_philodata *philo_data)
{
	char	sem_name[20];
	char	*num_str;
	int		i;

	philo_data->forks = malloc(sizeof(sem_t *) * philo_data->philo_nb);
	if (!philo_data->forks)
		return (0);
	i = 0;
	while (i < philo_data->philo_nb)
	{
		ft_memset(sem_name, 0, 20);
		sem_name[0] = '/';
		sem_name[1] = 'f';
		sem_name[2] = '_';
		num_str = ft_itoa(i);
		ft_strcat(sem_name, num_str);
		free(num_str);
		sem_unlink(sem_name);
		philo_data->forks[i] = sem_open(sem_name, O_CREAT, 0644, 1);
		i++;
	}
	return (1);
}

int	init_philo_data_basic(int ac, char **av, t_philodata *philo_data)
{
	philo_data->philo_nb = ft_atoi(av[1]);
	philo_data->time_to_die = ft_atoi(av[2]);
	philo_data->time_to_eat = ft_atoi(av[3]);
	philo_data->time_to_sleep = ft_atoi(av[4]);
	philo_data->must_eat = -1;
	if (ac == 6)
		philo_data->must_eat = ft_atoi(av[5]);
	philo_data->simulation_running = 1;
	return (1);
}

int	init_global_semaphores(t_philodata *philo_data)
{
	sem_unlink("/simulation");
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/status");
	sem_unlink("/meal");
	philo_data->simulation_sem = sem_open("/simulation", O_CREAT, 0644, 1);
	philo_data->forks_sem = sem_open("/forks", O_CREAT, 0644,
			philo_data->philo_nb);
	philo_data->write_sem = sem_open("/write", O_CREAT, 0644, 1);
	philo_data->status_sem = sem_open("/status", O_CREAT, 0644, 1);
	philo_data->meal_sem = sem_open("/meal", O_CREAT, 0644, 1);
	return (philo_data->simulation_sem != SEM_FAILED
		&& philo_data->forks_sem != SEM_FAILED
		&& philo_data->write_sem != SEM_FAILED
		&& philo_data->status_sem != SEM_FAILED
		&& philo_data->meal_sem != SEM_FAILED);
}

int	init_data_philo_bonus(int ac, char **av, t_philodata *philo_data)
{
	init_philo_data_basic(ac, av, philo_data);
	philo_data->philosophers = malloc(sizeof(t_philo) * philo_data->philo_nb);
	if (!philo_data->philosophers)
		return (0);
	if (!init_global_semaphores(philo_data))
	{
		free(philo_data->philosophers);
		return (0);
	}
	if (!init_fork_semaphores(philo_data))
	{
		free(philo_data->philosophers);
		return (0);
	}
	philo_data->start_time = get_time_ms();
	return (1);
}
