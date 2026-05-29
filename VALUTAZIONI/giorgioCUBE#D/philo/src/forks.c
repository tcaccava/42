/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:59:28 by gsabatin          #+#    #+#             */
/*   Updated: 2025/05/28 19:34:45 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <bits/pthreadtypes.h>

void	release_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->table->forks[philo->left_fork_id].mutex);
	pthread_mutex_unlock(&philo->table->forks[philo->right_fork_id].mutex);
}

static int	take_forks_order(t_philo *philo,
							pthread_mutex_t *first, pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	if (print_state(FORK, philo) != 0)
	{
		pthread_mutex_unlock(first);
		return (1);
	}
	pthread_mutex_lock(second);
	if (print_state(FORK, philo) != 0)
	{
		release_forks(philo);
		return (1);
	}
	return (0);
}

int	take_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->left_fork_id;
	right = philo->right_fork_id;
	if (philo->id % 2 == 0)
		return (take_forks_order(philo,
				&philo->table->forks[right].mutex,
				&philo->table->forks[left].mutex));
	else
		return (take_forks_order(philo,
				&philo->table->forks[left].mutex,
				&philo->table->forks[right].mutex));
}
