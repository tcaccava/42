/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:32:41 by lluciani          #+#    #+#             */
/*   Updated: 2025/06/23 15:33:07 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 5 && ac != 6)
	{
		write(2, "Error whith number of arguments\n", 32);
		return (1);
	}
	if (!check_input(av))
	{
		write(2, "Invalid input\n", 14);
		return (1);
	}
	init_table(&table, av);
	if (table.n_philo == 0 || table.time_to_die == 0 || table.time_to_eat == 0
		|| table.time_to_sleep == 0)
	{
		write(2, "Invalid input\n", 14);
		return (0);
	}
	create_thread(&table);
	free_all(&table);
	return (0);
}
