/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:13:03 by valucare          #+#    #+#             */
/*   Updated: 2026/07/27 16:44:50 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	spawn_philos(t_table *table, int *spawned)
{
	*spawned = 0;
	while (*spawned < table->seats)
	{
		if (pthread_create(&table->philos[*spawned].tid, NULL,
				life_routine, &table->philos[*spawned]))
			return (1);
		(*spawned)++;
	}
	return (0);
}

static void	join_philos(t_table *table, int spawned)
{
	int	i;

	i = 0;
	while (i < spawned)
	{
		pthread_join(table->philos[i].tid, NULL);
		i++;
	}
}

static int	run_table(t_table *table)
{
	pthread_t	watcher;
	int			spawned;

	if (spawn_philos(table, &spawned) || pthread_create(&watcher, NULL,
			watch_routine, table))
	{
		stop_sim(table);
		join_philos(table, spawned);
		return (printf("%s", ERR_THREAD), 1);
	}
	join_philos(table, table->seats);
	pthread_join(watcher, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_table	table;
	int		status;

	if (argc < 5 || argc > 6)
		return (printf("%s", ERR_ARGC), 1);
	if (check_args(argc, argv))
		return (1);
	if (setup_table(&table, argv))
		return (printf("%s", ERR_ALLOC), 1);
	status = run_table(&table);
	destroy_table(&table);
	return (status);
}
