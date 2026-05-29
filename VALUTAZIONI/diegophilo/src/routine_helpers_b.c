/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_helpers_b.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:09:20 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:09:23 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Set request flags for any missing adjacent fork.
 * Requests are written while both adjacent fork mutexes are held,
 * so another thread cannot observe partial request state.
 */
void	request_missing_forks(t_philo *philo)
{
	t_rules	*rules;
	int		left;
	int		right;

	rules = philo->rules;
	left = philo->left_fork_idx;
	right = philo->right_fork_idx;
	lock_two_forks(rules, left, right);
	if (rules->fork_owner[left] != philo->idx)
		rules->fork_requested[left] = 1;
	if (rules->fork_owner[right] != philo->idx)
		rules->fork_requested[right] = 1;
	unlock_two_forks(rules, left, right);
}

/*
 * After eating, any fork still owned by this philosopher becomes dirty.
 * Dirty forks can later be transferred when a neighbor requests them.
 */
void	mark_forks_dirty(t_philo *philo)
{
	t_rules	*rules;
	int		left;
	int		right;

	rules = philo->rules;
	left = philo->left_fork_idx;
	right = philo->right_fork_idx;
	lock_two_forks(rules, left, right);
	if (rules->fork_owner[left] == philo->idx)
		rules->fork_dirty[left] = 1;
	if (rules->fork_owner[right] == philo->idx)
		rules->fork_dirty[right] = 1;
	unlock_two_forks(rules, left, right);
}

/*
 * Wait for a duration while still serving fork transfer requests.
 * This prevents sleeping/thinking philosophers from blocking progress.
 */
void	wait_with_service(t_philo *philo, long wait_ms)
{
	long	start;

	start = get_time_ms();
	while (!is_stopped(philo->rules))
	{
		serve_fork_requests(philo);
		if ((get_time_ms() - start) >= wait_ms)
			break ;
		usleep(200);
	}
}

/*
 * Update local meal counter and global completion counter.
 * Each philosopher increments global finished_count only once,
 * when reaching the configured meal target for the first time.
 */
int	register_meal_if_full(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	if (philo->rules->must_eat_count != -1
		&& !philo->reported_full
		&& philo->meals_eaten >= philo->rules->must_eat_count)
	{
		philo->reported_full = 1;
		pthread_mutex_lock(&philo->rules->done_mutex);
		philo->rules->finished_count++;
		pthread_mutex_unlock(&philo->rules->done_mutex);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (0);
}

/*
 * Acquire both forks using request/serve rounds.
 * Loop stops when both forks are owned or global stop is raised.
 */
int	acquire_both_forks(t_philo *philo)
{
	while (!is_stopped(philo->rules))
	{
		if (owns_both_forks(philo))
			return (0);
		request_missing_forks(philo);
		serve_fork_requests(philo);
		usleep(200);
	}
	return (1);
}
