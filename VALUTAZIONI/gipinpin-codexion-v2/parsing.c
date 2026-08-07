/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:34 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:26:52 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	check_int(char *str)
{
	long	val;

	val = atol_check(str);
	if (val <= 0 || val > INT_MAX)
		return (-1);
	return (val);
}

static t_scheduler	parse_scheduler(char *str)
{
	if (!str)
		return (SCHED_INVALID);
	if (strcmp(str, "fifo") == 0)
		return (FIFO);
	if (strcmp(str, "edf") == 0)
		return (EDF);
	return (SCHED_INVALID);
}

t_sim	*validate_input(char **argv)
{
	t_sim	*sim;

	sim = init_sim();
	if (!sim)
		return (NULL);
	sim->n_coders = check_int(argv[0]);
	sim->time_to_burnout = check_int(argv[1]);
	sim->time_to_compile = atol_check(argv[2]);
	sim->time_to_debug = atol_check(argv[3]);
	sim->time_to_refactor = atol_check(argv[4]);
	sim->n_compiles_required = atol_check(argv[5]);
	sim->dongle_cooldown = atol_check(argv[6]);
	sim->scheduler = parse_scheduler(argv[7]);
	if (sim->n_coders == -1
		|| sim->time_to_burnout < 0
		|| sim->time_to_compile < 0
		|| sim->time_to_debug < 0
		|| sim->time_to_refactor < 0
		|| sim->n_compiles_required < 0
		|| sim->dongle_cooldown < 0
		|| sim->scheduler == SCHED_INVALID)
		return (free(sim), NULL);
	return (sim);
}
