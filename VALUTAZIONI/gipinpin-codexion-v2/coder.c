/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:33 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:33 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_all_coders(t_coder *l_coders, int count)
{
	int	i;

	if (!l_coders)
		return ;
	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&l_coders[i].coder_mutex);
		i++;
	}
	free(l_coders);
}

static void	coder_compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->coder_mutex);
	coder->is_compiling = 1;
	coder->last_compile_time = get_current_time_ms();
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->coder_mutex);
	log_status(coder, "is compiling");
	custom_usleep(coder, coder->sim->time_to_compile);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->is_compiling = 0;
	pthread_mutex_unlock(&coder->coder_mutex);
}

static void	action_of_routine(t_coder *coder)
{
	if (coder->sim->n_coders == 1)
	{
		if (!acquire_both_dongles(coder, coder->left_dongle))
			return ;
		custom_usleep(coder, coder->sim->time_to_burnout + 10);
		release_dongle(coder->left_dongle, coder->sim);
		return ;
	}
	if (!acquire_both_dongles(coder, coder->left_dongle))
		return ;
	coder_compile(coder);
	release_dongle(coder->left_dongle, coder->sim);
	release_dongle(coder->right_dongle, coder->sim);
	log_status(coder, "is debugging");
	custom_usleep(coder, coder->sim->time_to_debug);
	log_status(coder, "is refactoring");
	custom_usleep(coder, coder->sim->time_to_refactor);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (check_sim_stop(coder->sim))
		return (NULL);
	if (coder->id % 2 == 0)
		custom_usleep(coder, coder->sim->time_to_compile / 2);
	while (!check_sim_stop(coder->sim))
		action_of_routine(coder);
	return (NULL);
}

int	init_sim_coders(t_sim *sim)
{
	int	i;

	sim->coders = (t_coder *) ft_calloc(sim->n_coders, sizeof(t_coder));
	if (!sim->coders)
		return (0);
	i = -1;
	while (++i < sim->n_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		if (i == sim->n_coders - 1)
		{
			sim->coders[i].left_dongle = &sim->dongles[(i + 1) % sim->n_coders];
			sim->coders[i].right_dongle = &sim->dongles[i];
		}
		else
		{
			sim->coders[i].left_dongle = &sim->dongles[i];
			sim->coders[i].right_dongle = (
					&sim->dongles[(i + 1) % sim->n_coders]);
		}
		if (pthread_mutex_init(&sim->coders[i].coder_mutex, NULL) != 0)
			return (0);
	}
	return (1);
}
