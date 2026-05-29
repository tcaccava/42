/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:05:40 by gsabatin          #+#    #+#             */
/*   Updated: 2025/05/22 16:14:44 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long long	time_diff(long long past, long long present)
{
	return (present - past);
}

long long	get_time_ms(t_table *table_input)
{
	struct timeval	tv;
	static t_table	*table;

	if (!table)
		table = table_input;
	if (gettimeofday(&tv, NULL))
		clean_exit(NULL, "Time fetch failed.", 0);
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
