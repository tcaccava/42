/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:02:18 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/07/14 19:23:09 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	request_dangle(t_global_data *data, t_coder *coder)
{
	t_request	request;

	pthread_mutex_lock(&data->mutex);
	request.sended_time = get_now_time_ms();
	request.deadline = coder->last_compile_init_time + data->time_to_burnout;
	request.coder_id = coder->id;
	push_to_heap(data->heap, &request, data->scheduler);
	while (!coder->has_dungles && !data->stop_simulation)
		pthread_cond_wait(&coder->cond, &data->mutex);
	if (data->stop_simulation)
	{
		pthread_mutex_unlock(&data->mutex);
		return (0);
	}
	coder->last_compile_init_time = get_now_time_ms();
	pthread_mutex_unlock(&data->mutex);
	return (1);
}

static void	release_dangle(t_global_data *data, int coder_id)
{
	int	left_id;
	int	right_id;

	pthread_mutex_lock(&data->mutex);
	left_id = coder_id;
	right_id = (coder_id + 1) % data->number_of_coders;
	data->dungles[left_id].is_in_use = 0;
	data->dungles[right_id].is_in_use = 0;
	data->coders[coder_id].has_dungles = 0;
	data->dungles[left_id].last_used = get_now_time_ms();
	data->dungles[right_id].last_used = get_now_time_ms();
	pthread_mutex_unlock(&data->mutex);
}

static int	stop_simulation(t_global_data *data)
{
	pthread_mutex_lock(&data->mutex);
	if (data->stop_simulation)
	{
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->mutex);
	return (0);
}

static void	compile_cycle(t_global_data *data, t_coder *coder)
{
	log_event(data, get_now_time_ms() - data->simulation_start,
		coder->id + 1, "has taken a dongle");
	log_event(data, get_now_time_ms() - data->simulation_start,
		coder->id + 1, "has taken a dongle");
	log_event(data, get_now_time_ms() - data->simulation_start,
		coder->id + 1, "is compiling");
	ft_sleep_ms(data->time_to_compile);
	pthread_mutex_lock(&data->mutex);
	coder->compile_dones++;
	coder->last_compile_time = get_now_time_ms();
	pthread_mutex_unlock(&data->mutex);
	release_dangle(data, coder->id);
	log_event(data, get_now_time_ms() - data->simulation_start,
		coder->id + 1, "is debugging");
	ft_sleep_ms(data->time_to_debug);
	log_event(data, get_now_time_ms() - data->simulation_start,
		coder->id + 1, "is refactoring");
	ft_sleep_ms(data->time_to_refactor);
}

void	*coder_routine(void *args)
{
	t_global_data	*data;
	t_coder			*coder;
	t_coder_args	*par;

	par = (t_coder_args *)args;
	data = par->global_data;
	coder = &data->coders[par->coder_id];
	free(par);
	coder->last_compile_time = get_now_time_ms();
	while (coder->compile_dones < data->number_of_compiles_required)
	{
		if (stop_simulation(data))
			break ;
		if (!request_dangle(data, coder))
			break ;
		compile_cycle(data, coder);
		if (stop_simulation(data))
			break ;
	}
	return (NULL);
}
