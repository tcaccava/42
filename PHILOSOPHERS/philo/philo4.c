/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:57:21 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/24 15:38:08 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_stop_simulation(t_program *program, int value)
{
	pthread_mutex_lock(&program->stop_mutex);
	program->stop_simulation = value;
	pthread_mutex_unlock(&program->stop_mutex);
}

void	check_all_fed(t_program *program)
{
	if (!get_stop_simulation(program) && program->time_must_eat != -1
		&& program->all_finished)
	{
		pthread_mutex_lock(&program->print_mutex);
		printf("%ld All philosophers have eaten enough\n", get_time());
		set_stop_simulation(program, 1);
		pthread_mutex_unlock(&program->print_mutex);
	}
}
