/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:15:58 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 15:16:14 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)b;
	while (i < len)
		ptr[i++] = (unsigned char)c;
	return (b);
}

int	check_simulation_status_bonus(t_philodata *data)
{
	int	status;

	sem_wait(data->status_sem);
	status = data->simulation_running;
	sem_post(data->status_sem);
	return (status);
}

void	cleanup_forks(t_philodata *philo_data)
{
	int		i;
	char	sem_name[20];
	char	*num_str;

	i = 0;
	while (i < philo_data->philo_nb)
	{
		sem_close(philo_data->forks[i]);
		ft_memset(sem_name, 0, 20);
		sem_name[0] = '/';
		sem_name[1] = 'f';
		sem_name[2] = '_';
		num_str = ft_itoa(i);
		ft_strcat(sem_name, num_str);
		free(num_str);
		sem_unlink(sem_name);
		i++;
	}
}

void	cleanup_resources(t_philodata *philo_data)
{
	cleanup_forks(philo_data);
	sem_close(philo_data->simulation_sem);
	sem_close(philo_data->forks_sem);
	sem_close(philo_data->write_sem);
	sem_close(philo_data->status_sem);
	sem_close(philo_data->meal_sem);
	sem_unlink("/simulation");
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/status");
	sem_unlink("/meal");
	free(philo_data->forks);
	free(philo_data->philosophers);
	free(philo_data);
}
