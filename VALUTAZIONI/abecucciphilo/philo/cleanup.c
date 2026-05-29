/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abecucci <abecucci@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 16:31:09 by abecucci          #+#    #+#             */
/*   Updated: 2026/03/03 14:09:28 by abecucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	clean_philos(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->sh->n)
	{
		pthread_mutex_destroy(&philo[i].eat_count_mtx);
		pthread_mutex_destroy(&philo[i].last_meal_mtx);
		i++;
	}
}

static void	clean_shared(t_shared *sh)
{
	int	i;

	if (sh->forks)
	{
		i = 0;
		while (i < sh->n)
			pthread_mutex_destroy(&sh->forks[i++]);
		free(sh->forks);
		sh->forks = NULL;
	}
	pthread_mutex_destroy(&sh->print_mtx);
	pthread_mutex_destroy(&sh->start_cond_mtx);
	pthread_mutex_destroy(&sh->stop_mtx);
}

void	cleanup(t_shared *sh, t_philo **philos)
{
	if (!sh)
		return ;
	clean_shared(sh);
	if (!philos || !*philos)
		return ;
	clean_philos(*philos);
	free(*philos);
	*philos = NULL;
}

int	cleanup_fail(t_shared *sh, t_philo **philos)
{
	cleanup(sh, philos);
	return (EXIT_FAILURE);
}
