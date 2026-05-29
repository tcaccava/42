/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdonato <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:27:12 by tdonato           #+#    #+#             */
/*   Updated: 2025/02/19 20:07:20 by tdonato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wrong_args(void)
{
	printf("Input not valid\n");
	printf("This program takes as argument 4 or 5 positive integers\n");
	printf("exiting...\n");
}

int	verify(char *str, int	*valid, int equal)
{
	int	value;

	value = ft_atoi(str);
	if (value > 0 || (value == 0 && equal))
		return (1);
	*valid = 0;
	return (0);
}

int	create_forks(t_data *info)
{
	int	i;

	i = 0;
	info->someone_died = 0;
	info->start = c_time();
	info->forks = malloc(info->philo_nb * sizeof(pthread_mutex_t));
	if (!info->forks)
		return (1);
	while (i < info->philo_nb)
		pthread_mutex_init(info->forks + i++, 0);
	pthread_mutex_init(&(info->someone_died_mutex), 0);
	pthread_mutex_init(&(info->writing_mutex), 0);
	return (-1);
}

int	get_input(int argc, char **argv, t_data *info, int valid)
{
	if (verify(argv[1], &valid, 0))
		info->philo_nb = ft_atoi(argv[1]);
	if (verify(argv[2], &valid, 0))
		info->time_to_die = ft_atoi(argv[2]);
	if (verify(argv[3], &valid, 0))
		info->time_to_eat = ft_atoi(argv[3]);
	if (verify(argv[4], &valid, 0))
		info->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		if (verify(argv[5], &valid, 1))
			info->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	}
	else
		info->number_of_times_each_philosopher_must_eat = -1;
	if (!valid)
		wrong_args();
	if (!info->number_of_times_each_philosopher_must_eat)
		return (0);
	return (create_forks(info));
}
