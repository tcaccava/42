/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 18:37:30 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/07/15 10:56:59 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongles_available(t_global_data *g, int left_id, int right_id)
{
	long	now;

	if (left_id == right_id)
		return (0);
	now = get_now_time_ms();
	if (g->dungles[left_id].is_in_use
		|| g->dungles[right_id].is_in_use)
		return (0);
	if (now - g->dungles[left_id].last_used < g->dongle_cooldown)
		return (0);
	if (now - g->dungles[right_id].last_used < g->dongle_cooldown)
		return (0);
	return (1);
}

int	all_coders_done(t_global_data *g)
{
	int	i;

	i = -1;
	while (++i < g->number_of_coders)
	{
		if (g->coders[i].compile_dones
			< g->number_of_compiles_required)
			return (0);
	}
	return (1);
}

int	coder_time_expired(t_global_data *g)
{
	int	i;

	i = -1;
	while (++i < g->number_of_coders)
	{
		if (g->coders[i].compile_dones
			>= g->number_of_compiles_required)
			continue ;
		if (get_now_time_ms() - g->coders[i].last_compile_init_time
			> g->time_to_burnout)
			return (i);
	}
	return (-1);
}

void	check_expired(t_global_data *g, int *expired)
{
	*expired = coder_time_expired(g);
	if (*expired != -1)
	{
		log_event(g, get_now_time_ms() - g->simulation_start,
			*expired + 1, "burned out");
		g->stop_simulation = 1;
	}
}
