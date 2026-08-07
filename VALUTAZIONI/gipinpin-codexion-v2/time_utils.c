/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:34 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:34 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_current_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		putstr_fd(2, "Error in gettimeofday\n");
		return (-1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	custom_usleep(t_coder *coder, long time_to_wait)
{
	long	rem;
	long	start_time;
	long	curr_time;

	start_time = get_current_time_ms();
	if (start_time == -1)
		return ;
	while (1)
	{
		if (check_sim_stop(coder->sim))
			break ;
		curr_time = get_current_time_ms();
		if (curr_time == -1)
		{
			set_sim_stop(coder->sim, 1);
			return ;
		}
		rem = time_to_wait - (curr_time - start_time);
		if (rem <= 0)
			break ;
		else if (rem > 10)
			usleep(5000);
		else
			usleep(500);
	}
}
