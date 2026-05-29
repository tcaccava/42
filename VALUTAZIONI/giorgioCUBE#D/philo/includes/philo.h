/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:45:00 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 19:57:56 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "colors.h"

# include <stdbool.h>
# include <stdio.h>      /* printf */
# include <stdlib.h>     /* malloc, free */
# include <unistd.h>     /* write, usleep */
# include <string.h>     /* memset */
# include <sys/time.h>   /* gettimeofday */
# include <pthread.h>    /* pthread_create, pthread_detach, pthread_join
							pthread_mutex_init, pthread_mutex_destroy
							pthread_mutex_lock, pthread_mutex_unlock */

/* String macro */
# define USAGE "Usage: number_of_philosophers time_to_die time_to_eat \
	time_to_sleep [number_of_times_each_philosopher_must_eat]"
# define NO_MEALS_LIMIT -1

/**
 * @enum State of the philosopher
 *
 * @param DIE timestamp_in_ms X died
 * @param FORK timestamp_in_ms X has taken a fork
 * @param EAT timestamp_in_ms X is eating
 * @param SLEEP timestamp_in_ms X is sleeping
 * @param THINK timestamp_in_ms X is thinking
 */
typedef enum e_state
{
	DIE,
	FORK,
	EAT,
	SLEEP,
	THINK
}	t_state;

typedef enum e_mutex_id
{
	MEALS,
	TIME
}	t_mutexes_id;

typedef struct s_fork
{
	int				id;
	pthread_mutex_t	mutex;
}	t_fork;

/**
* @struct Philosopher data
*
* @param Philosopher number (1 to n)
* @param Left fork index
* @param Right fork index
* @param Timestamp of last meal
* @param Counter for meals eaten
* @param Shared data pointer
* @param Thread identifier
*/
typedef struct s_philo
{
	int				id;
	int				left_fork_id;
	int				right_fork_id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_mutex_t	mutexes[2];
	pthread_t		thread;
	struct s_table	*table;
}	t_philo;

/**
* @struct Table data
*
* @param n_philos Number of philosophers
* @param time_to_die Time until starvation (ms)
* @param time_to_eat Time spent eating (ms)
* @param time_to_sleep Time spent sleeping (ms)
* @param must_eat_times Optional: number of times each must eat
* @param someone_died Flag to stop simulation
* @param start_time Simulation start timestamp
* @param forks Array of fork mutexes
* @param print_mutex Mutex to ensure atomic printing
* @param death_mutex Mutex to protect the someone_died flag from
*					 race conditions
*/
typedef struct s_table
{
	int				n_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_times;
	bool			simulation_end;
	long long		start_time;
	struct s_fork	*forks;
	struct s_philo	*philos;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	simulation_end_mutex;
}	t_table;

/* Functions */
int			parse(t_table *table, int argc, char **argv);
int			init_table(t_table *table);
int			start_simulation(t_table *table, t_philo *philos);
void		clean_exit(t_table *table, const char *info);

/* Philo functons */
void		*philo_routine(void *args);
int			take_forks(t_philo *philo);
void		release_forks(t_philo *philo);
int			print_state(t_state state, t_philo *philo);
bool		is_simulation_ended(t_table *table);

/* Time */
long long	get_timestamp(t_table *table);
void		ft_usleep(long long ms);
long long	get_time_ms(t_table *table_input);
long long	time_diff(long long past, long long present);
long long	time_diff_philo(t_philo *philo, long long present);

/* Helpers */
void		ft_putendl_fd(char *s, int fd);
void		exit_err(const char *str);
void		*ft_calloc(size_t nmemb, size_t size);

#endif
