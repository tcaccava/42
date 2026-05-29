/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:09:30 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:09:32 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Create one philosopher thread per configured philosopher.
 * Before thread creation, each philosopher receives start_ms as
 * last_meal baseline, guarded by its meal mutex.
 * The function tracks how many threads were actually spawned.
 */
static int	create_philo_threads(t_rules *rules, int *created)
{
	int	i;

	i = 0;
	while (i < rules->philo_count)
	{
		pthread_mutex_lock(&rules->philos[i].meal_mutex);
		rules->philos[i].last_meal_ms = rules->start_ms;
		pthread_mutex_unlock(&rules->philos[i].meal_mutex);
		if (pthread_create(&rules->philos[i].thread, NULL,
				philo_routine, &rules->philos[i]) != 0)
		{
			set_stop(rules, 1);
			return (error_msg("Failed to create philosopher thread"));
		}
		(*created)++;
		i++;
	}
	return (0);
}

/*
 * Join exactly the number of philosopher threads that were created.
 * This keeps teardown deterministic even if startup aborted midway.
 */
static void	join_philo_threads(t_rules *rules, int created)
{
	int	i;

	i = 0;
	while (i < created)
	{
		pthread_join(rules->philos[i].thread, NULL);
		i++;
	}
}

/*
 * Start simulation orchestration:
 * - capture shared start timestamp,
 * - create philosopher workers,
 * - create monitor worker,
 * - join monitor then all philosophers.
 */
int	start_simulation(t_rules *rules)
{
	int	created;

	created = 0;
	rules->start_ms = get_time_ms();
	if (create_philo_threads(rules, &created) != 0)
	{
		join_philo_threads(rules, created);
		return (1);
	}
	if (pthread_create(&rules->monitor, NULL, monitor_routine, rules) != 0)
	{
		set_stop(rules, 1);
		join_philo_threads(rules, created);
		return (error_msg("Failed to create monitor thread"));
	}
	pthread_join(rules->monitor, NULL);
	join_philo_threads(rules, created);
	return (0);
}
