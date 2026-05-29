/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_status.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 17:45:13 by ilnassi           #+#    #+#             */
/*   Updated: 2026/02/25 19:16:29 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

/*Returns the current timestamp in milliseconds using gettimeofday().
Used to compute relative times for actions and death detection.*/
long long	get_time(void)
{
	struct timeval	clock_time;

	gettimeofday(&clock_time, NULL);
	return (clock_time.tv_sec * 1000 + clock_time.tv_usec / 1000);
}

/*Prints a philosopher action in a thread-safe way.
It first checks if the simulation is still running and no death occurred,
then locks print_mutex to avoid interleaved output.*/
void	print_plan(t_philo *philo, char *plan)
{
	long long	time;

	pthread_mutex_lock(&philo->data->sim_mutex);
	if (!philo->data->sim_running || philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->sim_mutex);
		return ;
	}
	pthread_mutex_lock(&philo->data->print_mutex);
	time = get_time() - philo->data->start_time;
	printf("%lld %d %s\n", time, philo->id, plan);
	pthread_mutex_unlock(&philo->data->print_mutex);
	pthread_mutex_unlock(&philo->data->sim_mutex);
}
