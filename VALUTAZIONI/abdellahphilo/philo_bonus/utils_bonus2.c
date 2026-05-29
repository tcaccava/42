/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:09:24 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 15:15:47 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	ft_atoi(const char *str)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}

void	ft_sleep(long long duration_ms)
{
	long long	start_time;
	long long	current_time;
	long long	elapsed_time;

	start_time = get_time_ms();
	current_time = get_time_ms();
	elapsed_time = current_time - start_time;
	while (1)
	{
		current_time = get_time_ms();
		elapsed_time = current_time - start_time;
		if (elapsed_time > duration_ms)
			break ;
		usleep(95);
	}
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_philodata *data, int id, char *message)
{
	int	running;

	sem_wait(data->write_sem);
	sem_wait(data->status_sem);
	running = data->simulation_running;
	sem_post(data->status_sem);
	if (running)
		printf("%lld %d %s\n", get_time_ms() - data->start_time, id, message);
	sem_post(data->write_sem);
}

char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	while (dest[i] != '\0')
		i++;
	j = 0;
	while (src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}
