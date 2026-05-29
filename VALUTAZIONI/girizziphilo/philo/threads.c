/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 10:45:00 by girizzi           #+#    #+#             */
/*   Updated: 2026/03/06 12:56:42 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_dead(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->n_philos)
	{
		pthread_mutex_lock(&program->meal_lock);
		if (get_time() - program->philos[i].last_meal >= program->t_die)
		{
			print_status(&program->philos[i], "died");
			pthread_mutex_lock(&program->dead_lock);
			program->dead_flag = 1;
			pthread_mutex_unlock(&program->dead_lock);
			pthread_mutex_unlock(&program->meal_lock);
			return (1);
		}
		pthread_mutex_unlock(&program->meal_lock);
		i++;
	}
	return (0);
}

static void	eat_routine(t_philo *philo)
{
	if (philo->program->n_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		ft_usleep(philo->program->t_die, philo->program);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->program->meal_lock);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->program->meal_lock);
	ft_usleep(philo->program->t_eat, philo->program);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(1, philo->program);
	while (1)
	{
		pthread_mutex_lock(&philo->program->dead_lock);
		if (philo->program->dead_flag)
		{
			pthread_mutex_unlock(&philo->program->dead_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->program->dead_lock);
		eat_routine(philo);
		print_status(philo, "is sleeping");
		ft_usleep(philo->program->t_sleep, philo->program);
		print_status(philo, "is thinking");
	}
	return (NULL);
}

static int	check_meals(t_program *program)
{
	int	i;
	int	full;

	i = 0;
	full = 0;
	if (program->must_eat == -1)
		return (0);
	while (i < program->n_philos)
	{
		pthread_mutex_lock(&program->meal_lock);
		if (program->philos[i].meals_eaten >= program->must_eat)
			full++;
		pthread_mutex_unlock(&program->meal_lock);
		i++;
	}
	if (full == program->n_philos)
	{
		pthread_mutex_lock(&program->dead_lock);
		program->dead_flag = 1;
		pthread_mutex_unlock(&program->dead_lock);
		return (1);
	}
	return (0);
}

int	init_simulation(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->n_philos)
	{
		if (pthread_create(&program->philos[i].thread, NULL, &philo_routine,
				&program->philos[i]))
			return (1);
		i++;
	}
	while (1)
	{
		if (check_dead(program) || check_meals(program))
			break ;
		usleep(1000);
	}
	return (0);
}
