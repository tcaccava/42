/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abecucci <abecucci@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 13:25:49 by abecucci          #+#    #+#             */
/*   Updated: 2026/03/02 19:51:49 by abecucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_bool	fork_taken(t_philo *p, int fork)
{
	pthread_mutex_lock(&p->sh->forks[fork]);
	if (should_stop(p->sh))
	{
		pthread_mutex_unlock(&p->sh->forks[fork]);
		return (False);
	}
	print_state(p, "has taken a fork");
	return (True);
}

static t_bool	take_forks(t_philo *p, int *first, int *second)
{
	if (p->id % 2 == 0)
	{
		*first = p->right;
		*second = p->left;
	}
	else
	{
		*first = p->left;
		*second = p->right;
	}
	if (!fork_taken(p, *first))
		return (False);
	if (p->sh->n == 1 || !fork_taken(p, *second))
	{
		pthread_mutex_unlock(&p->sh->forks[*first]);
		return (False);
	}
	return (True);
}

static void	update_last_meal(t_philo *p)
{
	pthread_mutex_lock(&p->last_meal_mtx);
	p->last_meal = time_now_ms();
	pthread_mutex_unlock(&p->last_meal_mtx);
}

static void	update_meals(t_philo *p)
{
	pthread_mutex_lock(&p->eat_count_mtx);
	p->eat_count++;
	pthread_mutex_unlock(&p->eat_count_mtx);
}

void	philosophers_loop(t_philo *p)
{
	int	first;
	int	second;

	while (!should_stop(p->sh))
	{
		if (take_forks(p, &first, &second) == False)
			break ;
		update_last_meal(p);
		print_state(p, "is eating");
		if (ft_msleep(p->sh, p->sh->t_eat) == True)
			update_meals(p);
		pthread_mutex_unlock(&p->sh->forks[second]);
		pthread_mutex_unlock(&p->sh->forks[first]);
		if (should_stop(p->sh))
			break ;
		print_state(p, "is sleeping");
		if (ft_msleep(p->sh, p->sh->t_sleep) == False)
			break ;
		print_state(p, "is thinking");
	}
}
