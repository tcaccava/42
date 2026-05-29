/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:07:56 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:07:59 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Destroy every initialized philosopher meal mutex.
 */
static void	destroy_meal_mutexes(t_rules *rules)
{
	int	i;

	if (!rules->philos)
		return ;
	i = 0;
	while (i < rules->meal_mutex_inited)
	{
		pthread_mutex_destroy(&rules->philos[i].meal_mutex);
		i++;
	}
}

/*
 * Destroy every initialized fork mutex.
 */
static void	destroy_fork_mutexes(t_rules *rules)
{
	int	i;

	if (!rules->forks)
		return ;
	i = 0;
	while (i < rules->fork_mutex_inited)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
}

/*
 * Destroy global core mutexes when they were initialized.
 */
static void	destroy_core_mutexes(t_rules *rules)
{
	if (!rules->core_mutex_inited)
		return ;
	pthread_mutex_destroy(&rules->print_mutex);
	pthread_mutex_destroy(&rules->stop_mutex);
	pthread_mutex_destroy(&rules->done_mutex);
}

/*
 * Destroy initialized mutexes and free all heap allocations.
 * Init counters make destruction safe even after partial init failure.
 * This is the single teardown path used by every exit branch.
 */
void	cleanup_rules(t_rules *rules)
{
	destroy_meal_mutexes(rules);
	destroy_fork_mutexes(rules);
	destroy_core_mutexes(rules);
	free(rules->philos);
	free(rules->forks);
	free(rules->fork_owner);
	free(rules->fork_dirty);
	free(rules->fork_requested);
}
