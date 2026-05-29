/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 15:18:08 by ilnassi           #+#    #+#             */
/*   Updated: 2026/02/16 17:14:43 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*It stays inactive for the specified number of milliseconds, keeping 
the simulation responsive. 
It wakes up periodically to check if the simulation is still running, 
allowing for early exit when the monitor interrupts the program.*/
static void	smart_sleep(long long ms, t_philo *philo)
{
	long long	start;

	start = get_time();
	while (get_time() - start < ms)
	{
		if (!sim_is_running(philo->data))
			return ;
		usleep(200);
	}
}

/*Prints the "thinking" state if the simulation is still running.
This function does not block and is used to model the philosopher cycle.*/
void	think(t_philo *philo)
{
	if (!sim_is_running(philo->data))
		return ;
	print_plan(philo, "is thinking");
}

/*Updates the philosopher's last_meal timestamp (protected by a mutex),
prints the "eating" state, increments the meals 
counter (protected by a mutex), and then sleeps for time_to_eat 
using smart_sleep.*/
void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->time_gnammy_mutex);
	philo->last_food_time = get_time();
	pthread_mutex_unlock(&philo->time_gnammy_mutex);
	print_plan(philo, "is eating");
	pthread_mutex_lock(&philo->gnammy_mutex);
	philo->food_eaten++;
	pthread_mutex_unlock(&philo->gnammy_mutex);
	smart_sleep(philo->data->time_to_eat, philo);
}

/*Prints the "sleeping" state if the simulation is still running,
then sleeps for time_to_sleep using smart_sleep.*/
void	sleep_philo(t_philo *philo)
{
	if (!sim_is_running(philo->data))
		return ;
	print_plan(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep, philo);
}
