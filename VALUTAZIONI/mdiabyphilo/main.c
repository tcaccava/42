/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiaby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 17:53:18 by mdiaby            #+#    #+#             */
/*   Updated: 2026/07/22 17:53:29 by mdiaby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	join_threads(t_table *table, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(table->eaters[i].thread, NULL);
		i++;
	}
}

static int	create_philosophers(t_table *table, int *created)
{
	int	i;

	i = 0;
	*created = 0;
	while (i < table->number_of_philosophers)
	{
		if (pthread_create(&table->eaters[i].thread, NULL,
				routine, &table->eaters[i]) != 0)
			return (1);
		(*created)++;
		i++;
	}
	return (0);
}

int	start_simulation(t_table *table)
{
	pthread_t	monitor_tid;
	int			created;

	if (create_philosophers(table, &created) != 0)
	{
		set_dead(table);
		join_threads(table, created);
		return (1);
	}
	if (pthread_create(&monitor_tid, NULL, monitor, table) != 0)
	{
		set_dead(table);
		join_threads(table, created);
		return (1);
	}
	join_threads(table, table->number_of_philosophers);
	pthread_join(monitor_tid, NULL);
	return (0);
}

int	main(int ac, char *av[])
{
	t_table	table;

	if (check_args(ac))
		return (1);
	if (littlechek_nogood(av))
		return (1);
	memset(&table, 0, sizeof(t_table));
	parse_paras(&table, ac, av);
	if (check_para(table))
		return (1);
	if (alloc_paras(&table) == 1)
	{
		clean_up(&table);
		return (1);
	}
	if (start_simulation(&table) == 1)
	{
		clean_up(&table);
		return (1);
	}
	clean_up(&table);
	return (0);
}
