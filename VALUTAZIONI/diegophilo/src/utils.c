/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:10:00 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:10:02 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Print a standardized error line and return failure code.
 */
int	error_msg(const char *msg)
{
	printf("Error: %s\n", msg);
	return (1);
}

/*
 * Strict ASCII decimal conversion.
 * Rejects empty strings, non-digit characters, and int overflow.
 * Sets *ok to 1 only for successful full-string parsing.
 */
long	ft_atol_strict(const char *str, int *ok)
{
	long	res;
	int		i;

	*ok = 0;
	if (!str || !str[0])
		return (0);
	i = 0;
	res = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		if (res > 2147483647L / 10
			|| (res == 2147483647L / 10 && (str[i] - '0') > 7))
			return (0);
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	*ok = 1;
	return (res);
}

/*
 * Read global stop flag under mutex protection.
 * This provides a synchronized view across all worker threads.
 */
int	is_stopped(t_rules *rules)
{
	int	stopped;

	pthread_mutex_lock(&rules->stop_mutex);
	stopped = rules->stop;
	pthread_mutex_unlock(&rules->stop_mutex);
	return (stopped);
}

/*
 * Publish a new global stop value under mutex protection.
 */
void	set_stop(t_rules *rules, int value)
{
	pthread_mutex_lock(&rules->stop_mutex);
	rules->stop = value;
	pthread_mutex_unlock(&rules->stop_mutex);
}

/*
 * Emit one log line with timestamp and philosopher id.
 * print_mutex keeps lines atomic and ordered at output level.
 * Non-forced logs are suppressed after stop is published.
 */
int	log_status(t_philo *philo, const char *msg, int force)
{
	long	timestamp;

	if (!force && is_stopped(philo->rules))
		return (1);
	pthread_mutex_lock(&philo->rules->print_mutex);
	if (!force && is_stopped(philo->rules))
	{
		pthread_mutex_unlock(&philo->rules->print_mutex);
		return (1);
	}
	timestamp = get_time_ms() - philo->rules->start_ms;
	printf("%ld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->rules->print_mutex);
	return (0);
}
