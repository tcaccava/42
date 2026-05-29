/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:56:23 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 19:22:44 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <semaphore.h>

static int	init_philos(t_table *table)
{
	int	i;

	table->philos = ft_calloc(table->n_philos, sizeof(t_philo));
	if (!table->philos)
		return (1);
	i = -1;
	while (++i < table->n_philos)
	{
		(table->philos)[i].id = i + 1;
		(table->philos)[i].last_meal_time = 0;
		(table->philos)[i].meals_eaten = 0;
		(table->philos)[i].sem_meal_time_n = ft_itoa(i + 1);
		sem_unlink((table->philos)[i].sem_meal_time_n);
		(table->philos)[i].sem_meal_time = sem_open(
				(table->philos)[i].sem_meal_time_n, O_CREAT, 0644, 1);
		if ((table->philos)[i].sem_meal_time == SEM_FAILED)
			return (1);
		(table->philos)[i].table = table;
	}
	return (0);
}

static int	init_sems(t_table *table)
{
	t_sem_id	i;

	sem_unlink(SEM_FORKS_N);
	sem_unlink(SEM_PRINT_N);
	sem_unlink(SEM_MEALS_N);
	i = -1;
	while (++i < SEMS_NUM)
	{
		if (i == SEM_FORKS_ID)
			table->sems[i] = sem_open(SEM_FORKS_N,
					O_CREAT, 0644, table->n_philos);
		else if (i == SEM_PRINT_ID)
			table->sems[i] = sem_open(SEM_PRINT_N, O_CREAT, 0644, 1);
		else if (i == SEM_MEALS_ID)
			table->sems[i] = sem_open(SEM_MEALS_N, O_CREAT, 0644, 0);
		if (table->sems[i] == SEM_FAILED)
			return (1);
	}
	return (0);
}

int	init_table(t_table *table)
{
	if (init_sems(table) != 0)
		clean_exit(table, "Semaphores allocation failed", 0);
	if (init_philos(table) != 0)
		clean_exit(table, "Philo allocation failed", 0);
	return (0);
}
