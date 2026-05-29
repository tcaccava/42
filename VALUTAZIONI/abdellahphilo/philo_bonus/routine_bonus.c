/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:53:28 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 14:53:29 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	take_forks(t_philo *philo)
{
	t_philodata	*philo_data;
	int			left;
	int			right;

	philo_data = philo->philo_data;
	left = philo->fork_left;
	right = philo->fork_right;
	sem_wait(philo_data->forks[left]);
	print_status(philo_data, philo->id, "has taken a fork");
	sem_wait(philo_data->forks[right]);
	print_status(philo_data, philo->id, "has taken a fork");
}

static void	eat(t_philo *philo)
{
	t_philodata	*philo_data;

	philo_data = philo->philo_data;
	sem_wait(philo_data->status_sem);
	philo->last_meal_time = get_time_ms();
	sem_post(philo_data->status_sem);
	print_status(philo_data, philo->id, "is eating");
	ft_sleep(philo_data->time_to_eat);
	sem_wait(philo_data->status_sem);
	philo->meals_eaten++;
	sem_post(philo_data->status_sem);
}

void	sleep_and_think(t_philo *philo)
{
	t_philodata	*philo_data;

	philo_data = philo->philo_data;
	print_status(philo_data, philo->id, "is sleeping");
	ft_sleep(philo_data->time_to_sleep);
	print_status(philo_data, philo->id, "is thinking");
}

void	philosopher_routine_bonus(t_philo *philo)
{
	t_philodata	*philo_data;
	int			left;
	int			right;

	philo_data = philo->philo_data;
	left = philo->fork_left;
	right = philo->fork_right;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (check_simulation_status_bonus(philo_data))
	{
		take_forks(philo);
		eat(philo);
		sem_post(philo_data->forks[left]);
		sem_post(philo_data->forks[right]);
		if (philo_data->must_eat > 0
			&& philo->meals_eaten >= philo_data->must_eat)
			break ;
		sleep_and_think(philo);
	}
}
