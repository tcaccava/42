/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 18:42:00 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/07/14 18:42:12 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	error_exit(t_global_data *data, const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	free_all(data);
	return (1);
}

long	get_now_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	ft_sleep_ms(long ms)
{
	usleep(ms * 1000);
}
