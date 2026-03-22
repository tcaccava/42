/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:49:02 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/24 12:17:24 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		status;
	int		i;

	if (!parse_arguments(argc, argv, &data))
		return (1);
	if (!init_data(&data))
		return (1);
	start_simulation(&data);
	i = 0;
	status = 0;
	while (i < data.num_of_phil)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			terminate_all_processes(&data);
			break ;
		}
		i++;
	}
	cleanup(&data);
	return (0);
}
