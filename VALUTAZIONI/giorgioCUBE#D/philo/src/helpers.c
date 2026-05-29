/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:12:31 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 19:58:22 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"
#include "philo.h"
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

int	print_state(t_state state, t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print_mutex);
	if (state == DIE)
		printf(RED "%lld %d died\n" RST,
			get_timestamp(philo->table), philo->id);
	else if (is_simulation_ended(philo->table))
	{
		pthread_mutex_unlock(&philo->table->print_mutex);
		return (1);
	}
	else if (state == FORK)
		printf(YLW "%lld %d has taken a fork\n" RST,
			get_timestamp(philo->table), philo->id);
	else if (state == EAT)
		printf(GRN "%lld %d is eating\n" RST,
			get_timestamp(philo->table), philo->id);
	else if (state == SLEEP)
		printf(GRY "%lld %d is sleeping\n" RST,
			get_timestamp(philo->table), philo->id);
	else if (state == THINK)
		printf(CYN "%lld %d is thinking\n" RST,
			get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_mutex);
	return (0);
}

void	clean_exit(t_table *table, const char *str)
{
	int	i;

	i = -1;
	while (++i < table->n_philos)
	{
		pthread_mutex_destroy(&table->forks[i].mutex);
		pthread_mutex_destroy(&table->philos[i].mutexes[MEALS]);
		pthread_mutex_destroy(&table->philos[i].mutexes[TIME]);
	}
	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->simulation_end_mutex);
	if (table->forks)
		free(table->forks);
	if (table->philos)
		free(table->philos);
	if (str)
		exit_err(str);
}
