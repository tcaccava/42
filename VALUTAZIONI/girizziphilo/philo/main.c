/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 19:30:00 by girizzi           #+#    #+#             */
/*   Updated: 2026/03/15 19:37:34 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_program	program;

	if (argc != 5 && argc != 6)
	{
		write(2, "Error: wrong number of parameters\n", 35);
		return (1);
	}
	if (check_args(argv))
		return (1);
	if (init_program(&program, argv))
		return (1);
	if (init_forks(&program))
		return (free(program.philos), 1);
	init_philos(&program);
	if (init_simulation(&program))
		return (free_all(&program), 1);
	free_all(&program);
	return (0);
}

/*
Usage: ./philo <N1> <N2> <N3> <N4> <N5*>

N1: Number of philosophers (max 300)
N2: Time to die
N3: Time to eat
N4: Time to sleep
N5(*optional): Number of dinners required
*/