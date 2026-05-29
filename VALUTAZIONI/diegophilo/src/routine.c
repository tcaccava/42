/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:09:00 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:09:02 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Execute one full eat cycle.
 * Single-philosopher case waits until death after taking one fork.
 * Multi-philosopher case acquires ownership of both forks first,
 * updates last_meal, eats for time_to_eat, then marks forks dirty.
 */
static int	eat_once(t_philo *philo)
{
	if (philo->rules->philo_count == 1)
	{
		log_status(philo, "has taken a fork", 0);
		while (!is_stopped(philo->rules))
			usleep(500);
		return (1);
	}
	if (acquire_both_forks(philo) != 0)
		return (1);
	log_status(philo, "has taken a fork", 0);
	log_status(philo, "has taken a fork", 0);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_ms = get_time_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	log_status(philo, "is eating", 0);
	precise_sleep(philo->rules, philo->rules->time_to_eat);
	mark_forks_dirty(philo);
	serve_fork_requests(philo);
	register_meal_if_full(philo);
	return (0);
}

/*
 * Philosopher thread main loop.
 * Repeats eat -> sleep -> think while global stop is not set.
 * Even IDs get a tiny startup stagger to reduce initial contention.
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(500);
	while (!is_stopped(philo->rules))
	{
		if (philo->rules->must_eat_count != -1 && philo->reported_full)
		{
			wait_with_service(philo, 1);
			continue ;
		}
		if (eat_once(philo) != 0)
			break ;
		log_status(philo, "is sleeping", 0);
		wait_with_service(philo, philo->rules->time_to_sleep);
		log_status(philo, "is thinking", 0);
		wait_with_service(philo, 1);
	}
	return (NULL);
}
