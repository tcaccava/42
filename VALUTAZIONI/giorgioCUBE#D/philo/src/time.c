/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:14:30 by gsabatin          #+#    #+#             */
/*   Updated: 2025/05/28 20:26:58 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	time_diff(long long past, long long present)
{
	return (present - past);
}

long long	time_diff_philo(t_philo *philo, long long present)
{
	long long	past;

	pthread_mutex_lock(&philo->mutexes[TIME]);
	past = philo->last_meal_time;
	pthread_mutex_unlock(&philo->mutexes[TIME]);
	return (present - past);
}

long long	get_time_ms(t_table *table_input)
{
	struct timeval	tv;
	static t_table	*table;

	if (!table)
		table = table_input;
	if (gettimeofday(&tv, NULL))
		clean_exit(NULL, "Time fetch failed.");
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long long ms)
{
	long long	start;

	start = get_time_ms(NULL);
	while (get_time_ms(NULL) - start < ms)
		usleep(100);
}

long long	get_timestamp(t_table *table)
{
	return (time_diff(table->start_time, get_time_ms(NULL)));
}
