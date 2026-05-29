/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:08:12 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:08:14 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Initialize core mutexes used across the whole simulation:
 * - print mutex protects log line atomicity,
 * - stop mutex protects global stop publication,
 * - done mutex protects completed-philosopher accounting.
 */
static int	init_core_mutexes(t_rules *rules)
{
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0)
		return (error_msg("Failed to init print mutex"));
	if (pthread_mutex_init(&rules->stop_mutex, NULL) != 0)
		return (error_msg("Failed to init stop mutex"));
	if (pthread_mutex_init(&rules->done_mutex, NULL) != 0)
		return (error_msg("Failed to init done mutex"));
	rules->core_mutex_inited = 1;
	return (0);
}

/*
 * Allocate all dynamic arrays used by the simulation runtime.
 */
static int	alloc_arrays(t_rules *rules)
{
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->philo_count);
	rules->fork_owner = malloc(sizeof(int) * rules->philo_count);
	rules->fork_dirty = malloc(sizeof(int) * rules->philo_count);
	rules->fork_requested = malloc(sizeof(int) * rules->philo_count);
	rules->philos = malloc(sizeof(t_philo) * rules->philo_count);
	if (!rules->forks || !rules->fork_owner || !rules->fork_dirty
		|| !rules->fork_requested || !rules->philos)
		return (error_msg("Malloc failed"));
	return (0);
}

/*
 * Initialize per-fork mutex and ownership/request state.
 */
static int	init_fork_data(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->philo_count)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0)
			return (error_msg("Failed to init fork mutex"));
		rules->fork_mutex_inited++;
		if (i == rules->philo_count - 1)
			rules->fork_owner[i] = 0;
		else
			rules->fork_owner[i] = i;
		rules->fork_dirty[i] = 1;
		rules->fork_requested[i] = 0;
		i++;
	}
	return (0);
}

/*
 * Initialize per-philosopher fields and personal meal mutexes.
 */
static int	init_philo_data(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->philo_count)
	{
		rules->philos[i].idx = i;
		rules->philos[i].id = i + 1;
		rules->philos[i].meals_eaten = 0;
		rules->philos[i].reported_full = 0;
		rules->philos[i].last_meal_ms = 0;
		rules->philos[i].left_fork_idx = (i - 1 + rules->philo_count)
			% rules->philo_count;
		rules->philos[i].right_fork_idx = i;
		rules->philos[i].rules = rules;
		if (pthread_mutex_init(&rules->philos[i].meal_mutex, NULL) != 0)
			return (error_msg("Failed to init meal mutex"));
		rules->meal_mutex_inited++;
		i++;
	}
	return (0);
}

/*
 * Top-level initializer for all mandatory resources.
 * Core locks are built first, then dynamic arrays and per-object locks.
 * Any failure is immediately propagated to caller for unified cleanup.
 */
int	init_rules(t_rules *rules)
{
	if (init_core_mutexes(rules) != 0)
		return (1);
	if (alloc_arrays(rules) != 0)
		return (1);
	if (init_fork_data(rules) != 0)
		return (1);
	if (init_philo_data(rules) != 0)
		return (1);
	return (0);
}
