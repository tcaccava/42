/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:33 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:33 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_all_dongles(t_dongle *l_dongle, int count)
{
	int	i;

	if (!l_dongle)
		return ;
	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&l_dongle[i].mutex);
		pthread_cond_destroy(&l_dongle[i].cond);
		pq_destroy(l_dongle[i].wait_queue);
		i++;
	}
	free(l_dongle);
}

int	init_sim_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = (t_dongle *)ft_calloc(sim->n_coders, sizeof(t_dongle));
	if (!sim->dongles)
		return (0);
	i = -1;
	while (++i < sim->n_coders)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].wait_queue = pq_create(sim->n_coders);
		if (!sim->dongles[i].wait_queue)
			return (0);
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
		{
			pq_destroy(sim->dongles[i].wait_queue);
			return (0);
		}
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
		{
			pq_destroy(sim->dongles[i].wait_queue);
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			return (0);
		}
	}
	return (1);
}
