/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shared.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abecucci <abecucci@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 20:06:57 by abecucci          #+#    #+#             */
/*   Updated: 2026/03/04 21:10:47 by abecucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	parse_positive_u64(const char *s, uint64_t *out)
{
	uint64_t	acc;
	int			i;

	if (!s || !*s || !out)
		return (FAILURE);
	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '+')
		i++;
	if (s[i] == '-' || s[i] == '\0')
		return (FAILURE);
	acc = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (FAILURE);
		acc = acc * 10 + (s[i] - '0');
		if (acc > INT_MAX)
			return (FAILURE);
		i++;
	}
	*out = acc;
	return (SUCCESS);
}

static int	parse_args(t_shared *sh, int argc, char **argv)
{
	uint64_t	tmp;

	if (!sh || !argv || argc < 5 || argc > 6)
		return (FAILURE);
	if (parse_positive_u64(argv[1], &tmp) != SUCCESS)
		return (FAILURE);
	sh->n = (int)tmp;
	if (parse_positive_u64(argv[2], &sh->t_die) != SUCCESS
		|| parse_positive_u64(argv[3], &sh->t_eat) != SUCCESS
		|| parse_positive_u64(argv[4], &sh->t_sleep) != SUCCESS)
		return (FAILURE);
	if (sh->n < 1 || sh->t_die < 1 || sh->t_eat < 1 || sh->t_sleep < 1)
		return (FAILURE);
	if (argc == 6)
	{
		if (parse_positive_u64(argv[5], &tmp) != SUCCESS || tmp < 1)
			return (FAILURE);
		sh->must_eat_count = (int)tmp;
	}
	else
		sh->must_eat_count = -1;
	return (SUCCESS);
}

static int	init_forks(t_shared *sh)
{
	int	i;

	i = 0;
	while (i < sh->n)
	{
		if (pthread_mutex_init(&sh->forks[i], NULL) != SUCCESS)
		{
			while (i > 0)
				pthread_mutex_destroy(&sh->forks[--i]);
			free(sh->forks);
			sh->forks = NULL;
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

static int	init_mutexes(t_shared *sh)
{
	int	i;

	if (pthread_mutex_init(&sh->print_mtx, NULL) != SUCCESS
		|| pthread_mutex_init(&sh->start_cond_mtx, NULL) != SUCCESS
		|| pthread_mutex_init(&sh->stop_mtx, NULL) != SUCCESS)
	{
		i = 0;
		while (i < sh->n)
			pthread_mutex_destroy(&sh->forks[i++]);
		free(sh->forks);
		sh->forks = NULL;
		return (FAILURE);
	}
	return (SUCCESS);
}

int	init_shared(t_shared *sh, int argc, char **argv)
{
	if (!sh)
		return (FAILURE);
	if (parse_args(sh, argc, argv) != SUCCESS)
		return (FAILURE);
	sh->stop = False;
	sh->forks = (pthread_mutex_t *)malloc(sh->n * sizeof(pthread_mutex_t));
	if (!sh->forks)
		return (FAILURE);
	if (init_forks(sh) != SUCCESS)
		return (FAILURE);
	if (init_mutexes(sh) != SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}
