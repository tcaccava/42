/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:34 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:34 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_sim_stop(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->monitor_mutex);
	stop = sim->sim_stop;
	pthread_mutex_unlock(&sim->monitor_mutex);
	return (stop);
}

void	set_sim_stop(t_sim *sim, int n)
{
	if (!sim || (n != 0 && n != 1))
		return ;
	pthread_mutex_lock(&sim->monitor_mutex);
	sim->sim_stop = n;
	pthread_mutex_unlock(&sim->monitor_mutex);
}

t_sim	*init_sim(void)
{
	t_sim	*sim;

	sim = (t_sim *) ft_calloc(1, sizeof(t_sim));
	if (!sim)
		return (NULL);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (free(sim), NULL);
	if (pthread_mutex_init(&sim->monitor_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&sim->print_mutex), free(sim), NULL);
	return (sim);
}
