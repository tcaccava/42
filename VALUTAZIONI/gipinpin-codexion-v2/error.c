/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:33 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:33 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	finish_prog(t_sim *sim, char *msg, int exit_code)
{
	if (sim)
	{
		if (sim->coders)
			free_all_coders(sim->coders, sim->n_coders);
		if (sim->dongles)
			free_all_dongles(sim->dongles, sim->n_coders);
		pthread_mutex_destroy(&sim->print_mutex);
		pthread_mutex_destroy(&sim->monitor_mutex);
		free(sim);
	}
	if (msg && *msg)
		putstr_fd(2, msg);
	return (exit_code);
}
