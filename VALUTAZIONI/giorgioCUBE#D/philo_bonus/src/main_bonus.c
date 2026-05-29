/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:58:58 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/25 14:21:45 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_table	table;

	memset(&table, '\0', sizeof(t_table));
	if (parse(&table, argc, argv) != 0)
		exit_err(USAGE);
	init_table(&table);
	start_simulation(&table, table.philos);
	clean_exit(&table, NULL, 0);
}
