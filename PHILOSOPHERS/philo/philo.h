/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:05:23 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/25 09:33:18 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_program	t_program;

typedef struct s_philosopher
{
	int						is_eating;
	int						id;
	int						num_of_phil;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						time_must_eat;
	int						meals_eaten;
	long					last_meal_time;
	int						*stop_simulation;
	pthread_mutex_t			*forks;
	pthread_mutex_t			meal_mutex;
	t_program				*program;
}							t_philosopher;

typedef struct s_program
{
	t_philosopher			*philosophers;
	pthread_t				*threads;
	pthread_t				monitor;
	pthread_mutex_t			*forks;
	pthread_mutex_t			print_mutex;
	pthread_mutex_t			stop_mutex;
	int						num_of_phil;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						time_must_eat;
	int						stop_simulation;
	long					start_time;
	int						all_finished;
}							t_program;

long						get_time(void);
void						print_status(t_philosopher *phil, char *status);
void						take_forks(t_philosopher *phil);
void						release_forks(t_philosopher *phil);
void						*philosopher_routine(void *arg);
void						*monitor_routine(void *arg);
int							parse_arguments(int argc, char **argv,
								t_program *program);
int							initialize_program(t_program *program);
void						initialize_philosophers(t_program *program);
void						create_threads(t_program *program);
void						join_threads(t_program *program);
void						cleanup_program(t_program *program);
int							ft_atoi(char *s);
void						eat_sleep_think(t_philosopher *phil,
								int time_to_think);
void						check_death_full(t_philosopher *phil,
								t_program *program);
int							get_stop_simulation(t_program *program);
void						set_stop_simulation(t_program *program, int value);
void						check_all_fed(t_program *program);

#endif