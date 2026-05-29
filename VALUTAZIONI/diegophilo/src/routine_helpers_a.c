/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_helpers_a.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:09:09 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:09:13 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Given a fork index and one endpoint owner index,
 * return the opposite endpoint philosopher index.
 */
static int	other_end(t_rules *rules, int fork_idx, int owner)
{
	if (owner == fork_idx)
		return ((fork_idx + 1) % rules->philo_count);
	return (fork_idx);
}

/*
 * Lock two fork mutexes in ascending index order.
 * A fixed global order avoids lock-order inversion when two threads
 * inspect overlapping fork pairs.
 */
void	lock_two_forks(t_rules *rules, int a, int b)
{
	if (a < b)
	{
		pthread_mutex_lock(&rules->forks[a]);
		pthread_mutex_lock(&rules->forks[b]);
	}
	else
	{
		pthread_mutex_lock(&rules->forks[b]);
		pthread_mutex_lock(&rules->forks[a]);
	}
}

/*
 * Unlock the same two fork mutexes that were acquired by lock_two_forks.
 * The release order mirrors the lock path and keeps lock logic symmetric.
 */
void	unlock_two_forks(t_rules *rules, int a, int b)
{
	if (a < b)
	{
		pthread_mutex_unlock(&rules->forks[b]);
		pthread_mutex_unlock(&rules->forks[a]);
	}
	else
	{
		pthread_mutex_unlock(&rules->forks[a]);
		pthread_mutex_unlock(&rules->forks[b]);
	}
}

/*
 * Serve pending fork requests for both adjacent forks.
 * If this philosopher owns a requested dirty fork, transfer ownership,
 * reset dirty flag, and clear request flag.
 */
void	serve_fork_requests(t_philo *philo)
{
	t_rules	*rules;
	int		f_idx;

	rules = philo->rules;
	f_idx = philo->left_fork_idx;
	pthread_mutex_lock(&rules->forks[f_idx]);
	if (rules->fork_owner[f_idx] == philo->idx && rules->fork_requested[f_idx]
		&& rules->fork_dirty[f_idx])
	{
		rules->fork_owner[f_idx] = other_end(rules, f_idx, philo->idx);
		rules->fork_dirty[f_idx] = 0;
		rules->fork_requested[f_idx] = 0;
	}
	pthread_mutex_unlock(&rules->forks[f_idx]);
	f_idx = philo->right_fork_idx;
	pthread_mutex_lock(&rules->forks[f_idx]);
	if (rules->fork_owner[f_idx] == philo->idx && rules->fork_requested[f_idx]
		&& rules->fork_dirty[f_idx])
	{
		rules->fork_owner[f_idx] = other_end(rules, f_idx, philo->idx);
		rules->fork_dirty[f_idx] = 0;
		rules->fork_requested[f_idx] = 0;
	}
	pthread_mutex_unlock(&rules->forks[f_idx]);
}

/*
 * Atomically verify if current philosopher owns both adjacent forks.
 * Ownership is checked while both corresponding fork mutexes are held.
 */
int	owns_both_forks(t_philo *philo)
{
	t_rules	*rules;
	int		left;
	int		right;
	int		owns_both;

	rules = philo->rules;
	left = philo->left_fork_idx;
	right = philo->right_fork_idx;
	lock_two_forks(rules, left, right);
	owns_both = (rules->fork_owner[left] == philo->idx
			&& rules->fork_owner[right] == philo->idx);
	unlock_two_forks(rules, left, right);
	return (owns_both);
}
