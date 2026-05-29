/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:32:34 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 17:05:33 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_think(t_philo *philo)
{
	print_state(S_THINK, philo);
}

void	philo_sleep(t_philo *philo)
{
	print_state(S_SLEEP, philo);
	ft_usleep(philo->table->time_to_sleep);
}

void	take_forks(t_philo *philo)
{
	sem_wait(philo->table->sems[SEM_FORKS_ID]);
	print_state(S_FORK, philo);
	sem_wait(philo->table->sems[SEM_FORKS_ID]);
	print_state(S_FORK, philo);
}

void	philo_eat(t_philo *philo)
{
	take_forks(philo);
	print_state(S_EAT, philo);
	ft_usleep(philo->table->time_to_eat);
	release_forks(philo);
	sem_wait(philo->sem_meal_time);
	if (philo->last_meal_time == -1)
		exit(1);
	philo->last_meal_time = get_time_ms(NULL);
	sem_post(philo->sem_meal_time);
	philo->meals_eaten++;
	return ;
}

void	handle_single_philo(t_philo *philo)
{
	print_state(S_FORK, philo);
	ft_usleep(philo->table->time_to_die);
	print_state(S_DIE, philo);
	exit(0);
}
