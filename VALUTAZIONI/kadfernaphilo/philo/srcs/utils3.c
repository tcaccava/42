/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadferna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:12:26 by kadferna          #+#    #+#             */
/*   Updated: 2025/06/23 13:12:30 by kadferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

/*
tslm = time =_since_last_meal
d = data
pme = philo_meals_eaten
*/
int	check_philos(t_data *d, t_philo *philos, int *all_ate)
{
	int		i;
	long	tslm;
	int		pme;
	long	current_time;

	i = -1;
	*all_ate = 1;
	while (++i < d->num_philos)
	{
		current_time = get_time();
		pthread_mutex_lock(&d->meal_mutex);
		tslm = current_time - philos[i].last_meal_time;
		pme = philos[i].meals_eaten;
		pthread_mutex_unlock(&d->meal_mutex);
		if (tslm >= d->time_to_die - 2)
		{
			if (!check_death_flag(d, philos, tslm, i))
				return (0);
			continue ;
		}
		if (d->max_meals != -1 && pme < d->max_meals)
			*all_ate = 0;
		usleep(500);
	}
	return (1);
}

int	create_philos_routine(t_philo *philos, void *routine(void *), int i)
{
	if (pthread_create(&philos[i].thread, NULL, routine, &philos[i]) != 0)
		return (0);
	return (1);
}

t_data	*check_init(int argc, char **argv, int *nums, t_philo *philos)
{
	t_data	*data;

	if (!checks(nums, argc, argv))
		return (NULL);
	data = init(nums, &philos);
	if (!data)
		return (NULL);
	return (data);
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;
	size_t			i;

	if (n == 0)
		return (s);
	i = 0;
	p = (unsigned char *)s;
	while (i < n)
	{
		p[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

void	check_forks(t_philo *philo, pthread_mutex_t *first_fork,
		pthread_mutex_t *second_fork)
{
	int	left_fork_id;
	int	right_fork_id;

	left_fork_id = philo->id - 1;
	right_fork_id = philo->id % philo->data->num_philos;
	first_fork = second_fork;
	second_fork = first_fork;
	if (left_fork_id < right_fork_id)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
}
