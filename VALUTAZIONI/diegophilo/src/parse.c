/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:08:36 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:08:38 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Parse one positive integer argument from a string.
 * The conversion is strict: only decimal digits are accepted.
 * Zero or negative semantic values are rejected by design.
 */
static int	parse_positive_long(const char *str, long *out)
{
	int		ok;
	long	val;

	ok = 0;
	val = ft_atol_strict(str, &ok);
	if (!ok || val <= 0)
		return (1);
	*out = val;
	return (0);
}

/*
 * Parse CLI arguments for philosophers and timing values.
 * The function validates argument count and every numeric field,
 * including the optional "number_of_times_each_philosopher_must_eat".
 * Parsed values are stored directly in the shared rules struct.
 */
int	parse_args(t_rules *rules, int argc, char **argv)
{
	long	value;

	if (argc != 5 && argc != 6)
		return (error_msg(
				"Usage: ./philo number_of_philosophers time_to_die "
				"time_to_eat time_to_sleep "
				"[number_of_times_each_philosopher_must_eat]"));
	if (parse_positive_long(argv[1], &value) != 0)
		return (error_msg("Invalid number_of_philosophers"));
	rules->philo_count = (int)value;
	if (parse_positive_long(argv[2], &rules->time_to_die) != 0)
		return (error_msg("Invalid time_to_die"));
	if (parse_positive_long(argv[3], &rules->time_to_eat) != 0)
		return (error_msg("Invalid time_to_eat"));
	if (parse_positive_long(argv[4], &rules->time_to_sleep) != 0)
		return (error_msg("Invalid time_to_sleep"));
	if (argc == 6)
	{
		if (parse_positive_long(argv[5], &value) != 0)
			return (error_msg(
					"Invalid number_of_times_each_philosopher_must_eat"));
		rules->must_eat_count = (int)value;
	}
	return (0);
}
