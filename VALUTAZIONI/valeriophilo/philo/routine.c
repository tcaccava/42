/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:14:22 by valucare          #+#    #+#             */
/*   Updated: 2026/07/27 17:17:24 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_sleep(t_philo *philo)
{
	log_state(philo, "is sleeping");
	wait_ms(philo->table, philo->table->sleep_ms);
}

static void	philo_think(t_philo *philo)
{
	t_table	*table;
	long	idle;

	table = philo->table;
	log_state(philo, "is thinking");
	if (table->seats % 2 == 0)
		return ;
	idle = table->die_ms - table->eat_ms - table->sleep_ms;
	if (idle > 0)
		wait_ms(table, idle / 2);
}

static void	stagger_start(t_philo *philo)
{
	if (philo->rank % 2 == 0)
	{
		log_state(philo, "is thinking");
		wait_ms(philo->table, philo->table->eat_ms);
	}
}

void	*life_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	stagger_start(philo);
	while (!is_stopped(philo->table))
	{
		philo_eat(philo);
		if (is_stopped(philo->table))
			break ;
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
