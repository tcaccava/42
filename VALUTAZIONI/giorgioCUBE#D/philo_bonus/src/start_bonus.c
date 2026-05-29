/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:32:50 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 20:10:59 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	terminate_processes(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->n_philos)
	{
		if (table->philos[i].pid > 0)
			kill(table->philos[i].pid, SIGTERM);
	}
}

static int	launch_philos(t_table *table, t_philo *philos)
{
	int		i;
	pid_t	pid;

	i = -1;
	while (++i < table->n_philos)
	{
		philos[i].last_meal_time = table->start_time;
		pid = fork();
		if (pid < 0)
			return (1);
		else if (pid > 0)
			philos[i].pid = pid;
		if (pid == 0)
			philo_routine(table, &philos[i]);
	}
	return (0);
}

int	start_simulation(t_table *table, t_philo *philos)
{
	int	status;
	int	counter;

	table->start_time = get_time_ms(table);
	if (launch_philos(table, philos) != 0)
		clean_exit(table, "Philosopher launch failed.", 0);
	status = 0;
	counter = 0;
	while (1)
	{
		waitpid(-1, &status, 0);
		if (++counter == table->n_philos
			&& WIFEXITED(status) && WEXITSTATUS(status) == 0)
			break ;
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			continue ;
		else
			break ;
	}
	terminate_processes(table);
	return (0);
}
