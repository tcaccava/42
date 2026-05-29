/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:29:07 by rodolhop          #+#    #+#             */
/*   Updated: 2025/06/30 12:29:08 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_data *data, int id, char *status)
{
	long long	current_time;

	pthread_mutex_lock(&data->write_mutex);
	pthread_mutex_lock(&data->death_mutex);
	if (!data->someone_died)
	{
		current_time = get_time() - data->start_time;
		printf("%lld %d %s\n", current_time, id, status);
	}
	pthread_mutex_unlock(&data->death_mutex);
	pthread_mutex_unlock(&data->write_mutex);
}

int	ft_error(char *str)
{
	write(2, "Error: ", 7);
	while (*str)
		write(2, str++, 1);
	write(2, "\n", 1);
	return (ERROR);
}

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;
	int	num;

	if (argc != 5 && argc != 6)
		return (ft_error("Invalid number of arguments"));
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (ft_error("Arguments must be positive integers only"));
			j++;
		}
		num = ft_atoi(argv[i]);
		if (num <= 0)
			return (ft_error("Arguments must be positive integers"));
		i++;
	}
	return (SUCCESS);
}

void	free_all(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
		data->forks = NULL;
	}
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
}
