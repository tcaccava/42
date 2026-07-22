/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiaby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 17:56:39 by mdiaby            #+#    #+#             */
/*   Updated: 2026/07/22 17:56:42 by mdiaby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	int			id;
	int			n_time_eats;
	long		last_meal;
	int			is_full;
	pthread_t	thread;
	t_table		*table;
}	t_philo;

typedef struct s_table
{
	int				dead;
	int				full_count;
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	long			start_time;
	t_philo			*eaters;
	pthread_mutex_t	*forks;
	int				forks_init;
	int				printf_init;
	int				die_init;
	int				meal_init;
	pthread_mutex_t	lock_printf;
	pthread_mutex_t	lock_die;
	pthread_mutex_t	lock_meal;
}	t_table;

// main.c
int		start_simulation(t_table *table);

// parsing.c
int		ft_atoi(const char *str);
int		check_para(t_table table);
int		check_args(int ac);
int		littlechek_nogood(char **av);

// init.c
int		parse_paras(t_table *table, int ac, char **av);
int		alloc_paras(t_table *table);
void	clean_up(t_table *table);

// utils.c
long	get_time(void);
void	ft_usleep(long ms);
int		is_dead(t_table *table);
void	set_dead(t_table *table);
void	ft_print(t_table *table, int id, char *str);

// routine.c
void	*routine(void *arg);

// monitor.c
int		must_stop(t_table *table, t_philo *philo);
void	*monitor(void *arg);

#endif
