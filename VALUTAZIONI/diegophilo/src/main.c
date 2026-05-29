/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 13:08:20 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 13:08:22 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Zero the full runtime struct before parsing inputs.
 * This prevents stale stack values from influencing mutex counters,
 * pointers, timing fields, or stop flags.
 * The optional meal limit is then forced to -1 to mean "not requested".
 */
static void	zero_rules(t_rules *rules)
{
	int				i;
	unsigned char	*bytes;

	bytes = (unsigned char *)rules;
	i = 0;
	while (i < (int) sizeof(t_rules))
	{
		bytes[i] = 0;
		i++;
	}
	rules->must_eat_count = -1;
}

/*
 * Program entry point:
 * 1) initialize deterministic defaults,
 * 2) parse and validate arguments,
 * 3) initialize synchronization resources,
 * 4) start simulation and monitor,
 * 5) cleanup all allocated/initialized resources in every exit path.
 */
int	main(int argc, char **argv)
{
	t_rules	rules;

	zero_rules(&rules);
	if (parse_args(&rules, argc, argv) != 0)
		return (1);
	if (init_rules(&rules) != 0)
	{
		cleanup_rules(&rules);
		return (1);
	}
	if (start_simulation(&rules) != 0)
	{
		cleanup_rules(&rules);
		return (1);
	}
	cleanup_rules(&rules);
	return (0);
}
