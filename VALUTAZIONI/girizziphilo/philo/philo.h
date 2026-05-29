/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 19:30:40 by girizzi           #+#    #+#             */
/*   Updated: 2026/03/15 19:38:58 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>

# define PHILO_MAX 300

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	long long			last_meal;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	struct s_program	*program;
}	t_philo;

typedef struct s_program
{
	int				n_philos;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				must_eat;
	long long		start_time;
	int				dead_flag;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_program;

// check_args.c
int			check_args(char **argv);
int			ft_atoi(char *str);

// init.c
int			init_program(t_program *program, char **argv);
int			init_forks(t_program *program);
void		init_philos(t_program *program);

// utils.c
long long	get_time(void);
void		ft_usleep(long long time, t_program *program);
void		print_status(t_philo *philo, char *status);

// threads.c
int			init_simulation(t_program *program);
void		*philo_routine(void *arg);
int			check_dead(t_program *program);

// cleanup.c
void		free_all(t_program *program);

#endif
