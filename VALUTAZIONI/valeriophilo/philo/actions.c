/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:11:51 by valucare          #+#    #+#             */
/*   Updated: 2026/07/27 17:19:47 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	grab_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	log_state(philo, "has taken a fork");
	pthread_mutex_lock(philo->second_fork);
	log_state(philo, "has taken a fork");
}

static void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->second_fork);
	pthread_mutex_unlock(philo->first_fork);
}

static void	eat_meal(t_philo *philo)
{
	log_state(philo, "is eating");
	pthread_mutex_lock(&philo->table->state_mtx);
	philo->last_bite = now_ms();
	philo->meals_done++;
	pthread_mutex_unlock(&philo->table->state_mtx);
	wait_ms(philo->table, philo->table->eat_ms);
	drop_forks(philo);
}

static void	dine_alone(t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	log_state(philo, "has taken a fork");
	while (!is_stopped(philo->table))
		usleep(POLL_US);
	pthread_mutex_unlock(philo->first_fork);
}

void	philo_eat(t_philo *philo)
{
	if (philo->table->seats == 1)
	{
		dine_alone(philo);
		return ;
	}
	if (is_stopped(philo->table))
		return ;
	grab_forks(philo);
	if (is_stopped(philo->table))
	{
		drop_forks(philo);
		return ;
	}
	eat_meal(philo);
}
