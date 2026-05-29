/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadferna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:01:13 by kadferna          #+#    #+#             */
/*   Updated: 2025/07/23 10:01:10 by kadferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	philo = (t_philo *)arg;
	first_fork = philo->left_fork;
	second_fork = philo->right_fork;
	if (philo->id % 2 == 0)
		usleep(500);
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->death_flag)
			return (pthread_mutex_unlock(&philo->data->death_mutex), NULL);
		pthread_mutex_unlock(&philo->data->death_mutex);
		if (!pick_forks_and_eat(philo, first_fork, second_fork))
			break ;
		print_status(philo, "is sleeping");
		usleep(philo->data->time_to_sleep * 1000);
		print_status(philo, "is thinking");
		if (philo->data->num_philos % 2 == 1)
			usleep(50);
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	t_data	*data;
	t_philo	*philos;
	int		all_philos_have_eaten;

	philos = (t_philo *)arg;
	data = philos[0].data;
	while (1)
	{
		all_philos_have_eaten = 1;
		if (check_philos(data, philos, &all_philos_have_eaten) == 0)
			return (0);
		if (data->max_meals != -1 && all_philos_have_eaten)
		{
			pthread_mutex_lock(&data->death_mutex);
			data->death_flag = true;
			pthread_mutex_unlock(&data->death_mutex);
			return (NULL);
		}
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	int			nums[MAX_PHILOS];
	t_data		*data;
	t_philo		*philos;
	int			i;
	pthread_t	monitor_thread;

	i = -1;
	if (!checks(nums, argc, argv))
		return (1);
	data = init(nums, &philos);
	if (!data)
		return (1);
	while (++i < data->num_philos)
	{
		if (pthread_create(&philos[i].thread, NULL, routine, &philos[i]) != 0)
			return (1);
	}
	pthread_create(&monitor_thread, NULL, monitor, philos);
	i = -1;
	while (++i < data->num_philos)
		pthread_join(philos[i].thread, NULL);
	pthread_join(monitor_thread, NULL);
	if (data)
		free_data_struct(data, philos);
	return (0);
}
