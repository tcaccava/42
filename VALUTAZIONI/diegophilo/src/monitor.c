/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:08:29 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:08:31 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Scan all philosophers for starvation.
 * For each philosopher, read last_meal under meal mutex,
 * compare against time_to_die, and stop simulation on first death.
 */
static int	check_death(t_rules *rules)
{
	int		i;
	long	now;
	long	last_meal;

	now = get_time_ms();
	i = 0;
	while (i < rules->philo_count)
	{
		pthread_mutex_lock(&rules->philos[i].meal_mutex);
		last_meal = rules->philos[i].last_meal_ms;
		pthread_mutex_unlock(&rules->philos[i].meal_mutex);
		if ((now - last_meal) >= rules->time_to_die)
		{
			set_stop(rules, 1);
			log_status(&rules->philos[i], "died", 1);
			return (1);
		}
		i++;
	}
	return (0);
}

/*
 * Check global completion condition when meal target is enabled.
 * If all philosophers reported completion, publish stop.
 */
static int	check_completion(t_rules *rules)
{
	int	finished;

	if (rules->must_eat_count == -1)
		return (0);
	pthread_mutex_lock(&rules->done_mutex);
	finished = rules->finished_count;
	pthread_mutex_unlock(&rules->done_mutex);
	if (finished >= rules->philo_count)
	{
		set_stop(rules, 1);
		return (1);
	}
	return (0);
}

/*
 * Dedicated monitor thread loop.
 * Polls death first, then completion, with a short sleep to avoid busy spin.
 */
void	*monitor_routine(void *arg)
{
	t_rules	*rules;

	rules = (t_rules *)arg;
	while (!is_stopped(rules))
	{
		if (check_death(rules))
			break ;
		if (check_completion(rules))
			break ;
		usleep(1000);
	}
	return (NULL);
}
