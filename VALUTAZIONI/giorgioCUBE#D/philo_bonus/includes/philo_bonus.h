/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:58:59 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 19:21:37 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

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
# include <semaphore.h>

# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>

/* String macro */
# define USAGE "Usage: number_of_philosophers time_to_die time_to_eat \
	time_to_sleep [number_of_times_each_philosopher_must_eat]"
# define NO_MEALS_LIMIT -1

/* Semaphore names */
# define SEMS_NUM 3
# define SEM_FORKS_N "/sem_forks"
# define SEM_PRINT_N "/sem_print"
# define SEM_MEALS_N "/sem_meals"

typedef enum e_sem_id
{
	SEM_FORKS_ID,
	SEM_PRINT_ID,
	SEM_MEALS_ID
}	t_sem_id;

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
	S_DIE,
	S_FORK,
	S_EAT,
	S_SLEEP,
	S_THINK
}	t_state;

/**
* @struct Philosopher data
*
* @param Philosopher number (1 to n)
* @param Last meal timestamp
* @param Meals eaten counter
* @param Process ID
* @param Pointer to shared table data
*/
typedef struct s_philo
{
	int				id;
	long long		last_meal_time;
	int				meals_eaten;
	pid_t			pid;
	sem_t			*sem_meal_time;
	char			*sem_meal_time_n;
	struct s_table	*table;
	pthread_t		monitor_thread;
}	t_philo;

/**
* @struct Table data
*
* @param n_philos Number of philosophers
* @param time_to_die Time until starvation (ms)
* @param time_to_eat Time spent eating (ms)
* @param time_to_sleep Time spent sleeping (ms)
* @param must_eat_times Optional: number of times each must eat
* @param start_time Simulation start timestamp
* @param forks_sem Semaphore for forks
* @param print_sem Semaphore for atomic printing
* @param death_sem Semaphore for signaling death
* @param meals_sem Semaphore for tracking finished meals
* @param philos Array of philosopher data
*/
typedef struct s_table
{
	int			n_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			must_eat_times;
	long long	start_time;
	sem_t		*sems[3];
	t_philo		*philos;
}	t_table;

/* Functions */
int			parse(t_table *table, int argc, char **argv);
int			init_table(t_table *table);
int			start_simulation(t_table *table, t_philo *philos);
void		clean_exit(t_table *table, const char *str, bool is_child);

/* Philosopher functions */
void		philo_routine(t_table *table, t_philo *philo);
void		release_forks(t_philo *philo);

/* Philosopher actions */
void		philo_think(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		philo_eat(t_philo *philo);
void		handle_single_philo(t_philo *philo);

/* Time functions */
void		ft_usleep(long long ms);
long long	get_time_ms(t_table *table);
long long	time_diff(long long past, long long present);

/* Time */
long long	get_timestamp(t_table *table);
void		ft_usleep(long long ms);
long long	get_time_ms(t_table *table_input);
long long	time_diff(long long past, long long present);

/* Helpers */
void		ft_putendl_fd(char *s, int fd);
void		exit_err(const char *str);
void		*ft_calloc(size_t nmemb, size_t size);
void		print_state(t_state state, t_philo *philo);
int			launch_thread(pthread_t *monitor,
				void *(*routine)(void *), void *args);
char		*ft_itoa(int n);

#endif
