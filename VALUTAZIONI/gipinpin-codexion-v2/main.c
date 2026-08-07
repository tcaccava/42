/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:33 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:33 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_monitor(t_sim *sim)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	pthread_create(&monitor_thread, NULL, monitor_routine, sim);
	pthread_join(monitor_thread, NULL);
	while (i < sim->n_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	int			i;
	t_sim		*sim;

	if (argc != 9)
		return (
			finish_prog(NULL, "Invalid input, argc is not nine!\n", 1));
	sim = validate_input(&argv[1]);
	if (!sim)
		return (
			finish_prog(NULL, "Invalid arg values/allocation failed.\n", 1));
	if (!init_sim_dongles(sim) || !init_sim_coders(sim))
		return (
			finish_prog(sim, "Invalid arg values/allocation failed.\n", 1));
	i = 0;
	sim->start_time = get_current_time_ms();
	while (i < sim->n_coders)
	{
		sim->coders[i].last_compile_time = sim->start_time;
		pthread_create(
			&sim->coders[i].thread, NULL, coder_routine, &sim->coders[i]);
		i++;
	}
	init_monitor(sim);
	return (finish_prog(sim, NULL, 0));
}
