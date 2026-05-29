/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 02:21:25 by ilnassi           #+#    #+#             */
/*   Updated: 2026/02/25 19:14:36 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*Thread-safe getter for the global simulation flag.
Returns 1 if the simulation is running, 0 otherwise.*/
int	sim_is_running(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->sim_mutex);
	i = data->sim_running;
	pthread_mutex_unlock(&data->sim_mutex);
	return (i);
}

/*Locks the first fork according to an asymmetric strategy:
even philosophers lock the right fork first, 
odd philosophers lock the left fork.
This breaks symmetry and helps preventing circular wait (deadlock).*/
static void	lock_first(t_philo *philo)
{
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->right_fork);
	else
		pthread_mutex_lock(philo->left_fork);
	print_plan(philo, "has taken a fork");
}

/*Locks the second fork (the remaining one) after lock_first.
The order depends on philosopher parity to keep the 
asymmetric acquisition order.*/
static void	lock_second(t_philo *philo)
{
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->left_fork);
	else
		pthread_mutex_lock(philo->right_fork);
	print_plan(philo, "has taken a fork");
}

/* * Tries to acquire the two forks needed to eat.
Returns:
- 0 if no fork was taken (simulation stopped early),
- 1 if only one fork was taken (single philosopher case),
- 2 if both forks were successfully taken.
Also releases the first fork if the simulation stops 
between the two locks.
 */
int	take_forks(t_philo *philo)
{
	if (!sim_is_running(philo->data))
		return (0);
	lock_first(philo);
	if (philo->data->num_philos == 1)
		return (1);
	if (!sim_is_running(philo->data))
	{
		if (philo->id % 2 == 0)
			pthread_mutex_unlock(philo->right_fork);
		else
			pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	lock_second(philo);
	return (2);
}

/*Releases the forks previously taken by take_forks().
The forks_taken argument indicates how many 
forks must be released (0/1/2),
preventing unlocking forks that were never locked.*/
void	drop_forks(t_philo *philo, int forks_taken)
{
	if (forks_taken <= 0)
		return ;
	if (forks_taken == 1)
	{
		if (philo->id % 2 == 0)
			pthread_mutex_unlock(philo->right_fork);
		else
			pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
