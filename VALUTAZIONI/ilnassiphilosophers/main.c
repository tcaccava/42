/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 18:48:26 by ilnassi           #+#    #+#             */
/*   Updated: 2026/02/17 17:36:02 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include <string.h>

/*Entry point of the program.
Initializes the global simulation data, starts philosopher and monitor threads,
waits for all threads to finish, then destroys mutexes 
and frees allocated memory.
Returns 0 on success, 1 on error.*/
int	main(int ac, char **av)
{
	t_data		data;
	pthread_t	monitor;

	memset(&data, 0, sizeof(t_data));
	if (init_all(&data, ac, av))
		return (1);
	if (start_threads(&data, &monitor))
	{
		stop_sim(&data);
		join_threads(&data, monitor);
		destroy_all(&data);
		return (1);
	}
	join_threads(&data, monitor);
	destroy_all(&data);
	return (0);
}
