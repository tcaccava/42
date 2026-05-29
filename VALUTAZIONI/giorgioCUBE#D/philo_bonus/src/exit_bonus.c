/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:23:57 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 19:24:32 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	clear_sems(t_table *table, bool is_child)
{
	int	i;

	i = -1;
	while (++i < SEMS_NUM)
	{
		if (table->sems[i] && table->sems[i] != SEM_FAILED)
		{
			sem_close(table->sems[i]);
			table->sems[i] = NULL;
		}
	}
	if (!is_child)
	{
		sem_unlink(SEM_FORKS_N);
		sem_unlink(SEM_PRINT_N);
		sem_unlink(SEM_MEALS_N);
	}
}

void	clean_exit(t_table *table, const char *str, bool is_child)
{
	int	i;

	if (!table)
	{
		if (str)
			exit_err(str);
		return ;
	}
	clear_sems(table, is_child);
	if (!is_child && table->philos)
	{
		i = -1;
		while (++i < table->n_philos)
		{
			sem_close(table->philos[i].sem_meal_time);
			sem_unlink(table->philos[i].sem_meal_time_n);
			free(table->philos[i].sem_meal_time_n);
		}
		free(table->philos);
		table->philos = NULL;
	}
	if (str)
		exit_err(str);
}
