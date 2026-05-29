/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:16:26 by ilnassi           #+#    #+#             */
/*   Updated: 2026/02/17 15:21:55 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*Special case when there is only one philosopher.
The philosopher can take only one fork and will never be able to eat,
so the routine waits until the simulation stops, then releases the fork.*/
static void	one_philo_case(t_philo *philo)
{
	int	forks_taken;

	forks_taken = take_forks(philo);
	while (sim_is_running(philo->data))
		usleep(1000);
	drop_forks(philo, forks_taken);
}

/*Main philosopher thread routine.
After the start barrier, the philosopher cycles through:
- taking forks,
- eating,
- sleeping,
- thinking.
Includes small delays to reduce contention and improve 
scheduling stability.*/
void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		forks_taken;

	philo = (t_philo *)arg;
	wait_start(philo->data);
	if (philo->id % 2 == 0)
		usleep(1000);
	if (philo->data->num_philos == 1)
		return (one_philo_case(philo), NULL);
	while (sim_is_running(philo->data))
	{
		forks_taken = take_forks(philo);
		if (forks_taken != 2)
		{
			drop_forks(philo, forks_taken);
			break ;
		}
		eat(philo);
		drop_forks(philo, forks_taken);
		sleep_philo(philo);
		think(philo);
		if (philo->data->num_philos % 2 != 0)
			usleep(500);
	}
	return (NULL);
}
