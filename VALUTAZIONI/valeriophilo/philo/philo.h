/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:14:00 by valucare          #+#    #+#             */
/*   Updated: 2026/07/27 16:44:50 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define ERR_ARGC "Error: expected 4 or 5 arguments\n"
# define ERR_ARGV "Error: arguments must be positive integers\n"
# define ERR_ALLOC "Error: allocation failed\n"
# define ERR_THREAD "Error: thread creation failed\n"

# define INT_CEIL 2147483647
# define POLL_US 200

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				rank;
	int				meals_done;
	long			last_bite;
	pthread_t		tid;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	t_table			*table;
}	t_philo;

typedef struct s_table
{
	int				seats;
	long			die_ms;
	long			eat_ms;
	long			sleep_ms;
	int				meals_goal;
	long			epoch;
	int				stopped;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_mtx;
	pthread_mutex_t	state_mtx;
	pthread_mutex_t	stop_mtx;
	t_philo			*philos;
}	t_table;

/* parse.c */
long	parse_long(const char *str);
int		check_args(int argc, char **argv);

/* init.c */
int		setup_table(t_table *table, char **argv);
void	destroy_table(t_table *table);

/* utils.c */
long	now_ms(void);
int		is_stopped(t_table *table);
void	stop_sim(t_table *table);
void	wait_ms(t_table *table, long delay);
void	log_state(t_philo *philo, char *state);

/* actions.c */
void	philo_eat(t_philo *philo);

/* routine.c */
void	*life_routine(void *arg);

/* monitor.c */
void	*watch_routine(void *arg);

#endif
