/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:02:35 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/07/15 10:37:33 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_structs(t_global_data *data)
{
	int	i;

	data->heap->size = 0;
	i = -1;
	while (++i < data->number_of_coders)
	{
		data->coders[i].id = i;
		data->coders[i].has_dungles = 0;
		data->coders[i].compile_dones = 0;
		data->coders[i].last_compile_init_time = data->simulation_start;
		data->coders[i].last_compile_time = 0;
		if (pthread_cond_init(&data->coders[i].cond, NULL))
			return (0);
		data->dungles[i] = (t_dungle){i, 0, 0};
	}
	return (1);
}

int	init_memory(t_global_data *data)
{
	data->coders = NULL;
	data->dungles = NULL;
	data->heap = NULL;
	data->threads = NULL;
	if (pthread_mutex_init(&data->mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->log_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->mutex);
		return (0);
	}
	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	data->dungles = malloc(sizeof(t_dungle) * data->number_of_coders);
	data->threads = malloc(sizeof(pthread_t) * (data->number_of_coders + 1));
	data->heap = malloc(sizeof(t_heap));
	data->simulation_start = get_now_time_ms();
	if (!data->coders || !data->dungles || !data->heap || !data->threads)
		return (0);
	data->heap->requets = malloc(sizeof(t_request)
			* data->number_of_coders);
	if (!data->heap->requets)
		return (0);
	return (init_structs(data));
}

int	create_threads(t_global_data *data)
{
	int				i;
	int				ret;
	t_coder_args	*coder_args;

	i = -1;
	while (++i < data->number_of_coders)
	{
		coder_args = malloc(sizeof(*coder_args));
		if (!coder_args)
			return (0);
		*coder_args = (t_coder_args){i, data};
		ret = pthread_create(&data->threads[i], NULL,
				coder_routine, coder_args);
		if (ret)
		{
			free(coder_args);
			return (0);
		}
	}
	ret = pthread_create(&data->threads[i], NULL, monitor_runtime, data);
	if (ret)
		return (0);
	return (1);
}

int	join_threads(t_global_data *data)
{
	int	i;

	i = -1;
	while (++i <= data->number_of_coders)
	{
		if (pthread_join(data->threads[i], NULL) != 0)
			return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_global_data	data;

	if (!check_args(argc, argv, &data))
		return (1);
	if (!init_memory(&data))
		return (error_exit(&data,
				"ERROR: Failed to initialize memory"));
	if (!create_threads(&data))
		return (error_exit(&data,
				"ERROR: Failed to create threads"));
	if (!join_threads(&data))
		return (error_exit(&data,
				"ERROR: Failed to join threads"));
	free_all(&data);
	return (0);
}
