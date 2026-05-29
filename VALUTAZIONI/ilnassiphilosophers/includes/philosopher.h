/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:28:32 by ilnassi           #+#    #+#             */
/*   Updated: 2026/02/25 19:11:44 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_data	t_data;

/*Represents a philosopher thread.
Stores philosopher state (id, last meal time, meals count),
references to the two fork mutexes, and 
per-philosopher mutexes used to protect
shared fields accessed by both philosopher and monitor.*/
typedef struct s_philo
{
	int				id;
	int				food_eaten;
	long long		last_food_time;
	pthread_t		thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	gnammy_mutex;
	pthread_mutex_t	time_gnammy_mutex;
	t_data			*data;
}	t_philo;

/*Global simulation context shared by all threads.
Contains timing parameters, simulation flags, 
shared mutexes (print/sim/start),
the forks array (one mutex per fork), and the philosophers array.*/
typedef struct s_data
{
	int				num_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	long long		start_time;
	int				someone_died;
	int				sim_running;
	int				must_eat;
	int				start_flag;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
	pthread_mutex_t	sim_mutex;
	pthread_mutex_t	start_mutex;
	t_philo			*philos;
}	t_data;

/* routine */
void		think(t_philo *philo);
void		eat(t_philo *philo);
void		sleep_philo(t_philo *philo);

/* time / status */
long long	get_time(void);
int			check_death(t_philo *philo);
int			sim_is_running(t_data *data);

/* forks */
int			take_forks(t_philo *philo);
void		drop_forks(t_philo *philo, int forks_taken);

/* simulation init/run */
int			init_all(t_data *data, int argc, char **argv);
void		destroy_all(t_data *data);
int			start_threads(t_data *data, pthread_t *monitor);
void		join_threads(t_data *data, pthread_t monitor);
void		stop_sim(t_data *data);

/* threads */
void		*philo_routine(void *arg);
void		*monitor_sim(void *arg);

/* start barrier helpers */
void		wait_start(t_data *data);
void		set_start_time(t_data *data);

/* parsing / utils */
int			check_args(int argc, char **argv, t_data *data);
void		print_plan(t_philo *philo, char *plan);

#endif
