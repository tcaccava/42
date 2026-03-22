/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 11:53:50 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/12 19:25:22 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_program	program;

	memset(&program, 0, sizeof(t_program));
	if (!parse_arguments(argc, argv, &program))
		return (1);
	if (!initialize_program(&program))
		return (1);
	initialize_philosophers(&program);
	create_threads(&program);
	join_threads(&program);
	cleanup_program(&program);
	return (0);
}
