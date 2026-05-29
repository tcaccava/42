/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:08:27 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 19:24:32 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdint.h>

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
		write(fd, s++, 1);
	write(fd, "\n", 1);
}

void	exit_err(const char *str)
{
	printf(RED "%s\n" RST, str);
	exit(1);
}

/*
 * @brief Allocates memory for an array and initializes it to zero
 * @details Allocates memory for 'nmemb' elements of 'size' bytes each
 *          and initializes all bytes to zero. Checks for integer overflow
 *          before performing allocation.
 *
 * @param nmemb Number of elements to allocate
 * @param size Size of each element in bytes
 *
 * @return Pointer to allocated and zeroed memory, or NULL if allocation fails
*/
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (nmemb != 0 && size > SIZE_MAX / nmemb)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	memset(ptr, '\0', nmemb * size);
	return (ptr);
}

void	print_state(t_state state, t_philo *philo)
{
	unsigned long long	ts;

	ts = get_timestamp(philo->table);
	sem_wait(philo->table->sems[SEM_PRINT_ID]);
	if (state == S_DIE)
	{
		printf(RED "%lld %d died\n" RST, ts, philo->id);
		exit(1);
	}
	else if (state == S_FORK)
		printf(YLW "%lld %d has taken a fork\n" RST, ts, philo->id);
	else if (state == S_EAT)
		printf(GRN "%lld %d is eating\n" RST, ts, philo->id);
	else if (state == S_SLEEP)
		printf(GRY "%lld %d is sleeping\n" RST, ts, philo->id);
	else if (state == S_THINK)
		printf(CYN "%lld %d is thinking\n" RST, ts, philo->id);
	sem_post(philo->table->sems[SEM_PRINT_ID]);
	return ;
}

int	launch_thread(pthread_t *monitor, void *(*routine)(void *), void *args)
{
	{
		if (pthread_create(monitor, NULL,
				routine, args) != 0)
			return (1);
		if (pthread_detach(*monitor) != 0)
			return (1);
	}
	return (0);
}
