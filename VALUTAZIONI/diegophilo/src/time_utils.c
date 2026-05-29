/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:09:40 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:09:43 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Return current wall-clock timestamp in milliseconds.
 * Source is gettimeofday.
 */
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

/*
 * Sleep helper with cooperative stop checks.
 * Instead of one long usleep, this loops in small chunks so threads
 * can react quickly when stop becomes true.
 */
void	precise_sleep(t_rules *rules, long sleep_ms)
{
	long	start;

	start = get_time_ms();
	while (!is_stopped(rules))
	{
		if ((get_time_ms() - start) >= sleep_ms)
			break ;
		usleep(500);
	}
}
