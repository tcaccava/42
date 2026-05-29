/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:12:14 by rodolhop          #+#    #+#             */
/*   Updated: 2025/06/25 16:12:16 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <stdlib.h>
# include <limits.h>

# define SUCCESS 0
# define ERROR 1

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	int				left_fork;
	int				right_fork;
	long long		last_meal_time;
	pthread_t		thread;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_limit;
	long long		start_time;
	bool			someone_died;
	bool			all_ate_enough;
	pthread_t		monitor_thread;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_mutex;
	t_philo			*philos;
}	t_data;

/* init.c */
int			init_data(t_data *data, int argc, char **argv);
int			init_mutexes(t_data *data);
int			init_philos(t_data *data);

/* utils.c */
int			ft_error(char *str);
int			check_args(int argc, char **argv);
void		free_all(t_data *data);
void		print_status(t_data *data, int id, char *status);
int			ft_atoi(const char *str);
size_t		ft_strlen(const char *s);

/* time.c */
long long	get_time(void);
void		ft_sleep(long long time_to_sleep);

/* philo.c */
void		*philo_routine(void *arg);
void		take_forks(t_philo *philo);
void		eat(t_philo *philo);
void		sleep_and_think(t_philo *philo);

/* monitor.c */
int			check_if_dead(t_data *data);
int			check_if_all_ate(t_data *data);
void		*monitor_routine(void *arg);

/* main.c */
int			create_threads(t_data *data);
int			join_threads(t_data *data);

#endif