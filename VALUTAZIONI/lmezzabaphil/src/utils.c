/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:02:46 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/07/15 10:56:22 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_safe_atoi(const char *s, int *out)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	if (!s || !s[0])
		return (0);
	if (s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		result = result * 10 + (s[i] - '0');
		if (result > INT_MAX)
			return (0);
		i++;
	}
	*out = (int)result;
	return (1);
}

int	ft_safe_atol(const char *s, long *out)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	if (!s || !s[0])
		return (0);
	if (s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		if (result > (LONG_MAX - (s[i] - '0')) / 10)
			return (0);
		result = result * 10 + (s[i] - '0');
		i++;
	}
	*out = result;
	return (1);
}

void	log_event(t_global_data *data, long timestamp,
		int coder_id, const char *msg)
{
	pthread_mutex_lock(&data->log_mutex);
	if (!data->stop_simulation)
		printf("%ld %d %s\n", timestamp, coder_id, msg);
	pthread_mutex_unlock(&data->log_mutex);
}

void	free_all(t_global_data *data)
{
	int	i;

	if (data->coders)
	{
		i = -1;
		while (++i < data->number_of_coders)
			pthread_cond_destroy(&data->coders[i].cond);
	}
	pthread_mutex_destroy(&data->mutex);
	pthread_mutex_destroy(&data->log_mutex);
	if (data->heap)
	{
		free(data->heap->requets);
		free(data->heap);
	}
	free(data->coders);
	free(data->dungles);
	free(data->threads);
}
