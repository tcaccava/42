/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abecucci <abecucci@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 20:45:32 by abecucci          #+#    #+#             */
/*   Updated: 2026/03/03 20:19:00 by abecucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_bool	reached_meal_count(t_philo *philo)
{
	t_bool	full;

	if (philo->sh->must_eat_count < 0)
		return (False);
	pthread_mutex_lock(&philo->eat_count_mtx);
	full = (philo->eat_count >= philo->sh->must_eat_count);
	pthread_mutex_unlock(&philo->eat_count_mtx);
	return (full);
}

static t_bool	all_philos_are_full(t_philo *philos)
{
	int		i;
	t_bool	full;

	full = True;
	i = 0;
	while (i < philos->sh->n && full)
	{
		full = reached_meal_count(&(philos[i]));
		i++;
	}
	return (full);
}

static t_bool	is_dead(t_philo *philo)
{
	uint64_t	now;
	t_bool		dead;

	now = time_now_ms();
	pthread_mutex_lock(&philo->last_meal_mtx);
	dead = (now - philo->last_meal >= philo->sh->t_die);
	pthread_mutex_unlock(&philo->last_meal_mtx);
	return (dead);
}

static void	check_if_someone_is_dead(t_philo *philos)
{
	int		i;
	t_bool	dead;

	dead = False;
	i = 0;
	while (i < philos->sh->n && !dead)
	{
		dead = is_dead(&(philos[i]));
		if (dead)
		{
			raise_stop(philos->sh);
			pthread_mutex_lock(&philos->sh->print_mtx);
			printf("%" PRIu64 " %d died\n", time_now_ms() - philos->sh->start_time,
				philos[i].id);
			pthread_mutex_unlock(&philos->sh->print_mtx);
		}
		i++;
	}
}

void	monitor_loop(t_philo *philos)
{
	while (!should_stop(philos->sh))
	{
		usleep(MONITOR_SLEEP_TIME_US);
		if (all_philos_are_full(philos))
			raise_stop(philos->sh);
		else
			check_if_someone_is_dead(philos);
	}
}
