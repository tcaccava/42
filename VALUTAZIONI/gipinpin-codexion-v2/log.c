/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:33 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:33 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_status(t_coder *coder, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&coder->sim->print_mutex);
	if (check_sim_stop(coder->sim))
	{
		pthread_mutex_unlock(&coder->sim->print_mutex);
		return ;
	}
	timestamp = get_current_time_ms() - coder->sim->start_time;
	putnbr_fd(1, timestamp);
	write(1, " ", 1);
	putnbr_fd(1, coder->id);
	write(1, " ", 1);
	putstr_fd(1, msg);
	write(1, "\n", 1);
	pthread_mutex_unlock(&coder->sim->print_mutex);
}
