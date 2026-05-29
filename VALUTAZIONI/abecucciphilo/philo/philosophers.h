/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abecucci <abecucci@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:40:28 by abecucci          #+#    #+#             */
/*   Updated: 2026/03/03 15:04:13 by abecucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

/* --- LIBRERIE --- */
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <stdint.h>
# include <inttypes.h>
# include "utils.h"

/* --- MACRO --- */
# define SUCCESS 0
# define FAILURE 1
# define MONITOR_SLEEP_TIME_US 1000

/* --- STRUTTURE --- */
typedef struct s_shared
{
	int						n;
	uint64_t				t_die;
	uint64_t				t_eat;
	uint64_t				t_sleep;
	int						must_eat_count;
	t_bool					stop;
	pthread_mutex_t			*forks;
	pthread_mutex_t			print_mtx;
	pthread_mutex_t			start_cond_mtx;
	pthread_mutex_t			stop_mtx;
	uint64_t				start_time;
}							t_shared;

typedef struct s_philo
{
	int						id;
	pthread_t				thread;
	int						left;
	int						right;
	int						eat_count;
	uint64_t				last_meal;
	pthread_mutex_t			eat_count_mtx;
	pthread_mutex_t			last_meal_mtx;
	t_shared				*sh;
}							t_philo;

/* --- FUNZIONI --- */
int							init_shared(t_shared *sh, int argc, char **argv);
int							init_philos(t_philo **philos, t_shared *sh);

int							start_simulation(t_philo *philos);

void						monitor_loop(t_philo *philos);

void						philosophers_loop(t_philo *p);

void						cleanup(t_shared *sh, t_philo **philos);
int							cleanup_fail(t_shared *sh, t_philo **philos);

#endif //PHILOSOPHERS_H