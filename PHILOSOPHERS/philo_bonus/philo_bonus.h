/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:47:29 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/24 12:11:00 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philosopher
{
	int					id;
	pid_t				pid;
	int					meals_eaten;
	long				last_meal_time;
	int					has_finished;
	int					stop_simulation;
	pthread_t			monitor_thread;
	struct s_data		*data;
}						t_philosopher;

typedef struct s_data
{
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					num_of_phil;
	int					times_must_eat;
	int					stop_simulation;
	long				start_time;
	sem_t				*done_eating;
	sem_t				*forks;
	sem_t				*print;
	sem_t				*stop;
	t_philosopher		*philos;
	pthread_t			meal_monitor_thread;
}						t_data;

int						parse_arguments(int argc, char **argv, t_data *data);
int						init_data(t_data *data);
void					start_simulation(t_data *data);
void					cleanup(t_data *data);
void					philosopher_routine(t_philosopher *phil);
long					get_time(void);
void					print_status(t_philosopher *phil, char *status);
void					wait_for_processes(t_data *data);
int						ft_atoi(char *s);
void					terminate_all_processes(t_data *data);
void					*monitor_routine(void *arg);
void					setup_monitor(t_philosopher *phil);
void					*meal_monitor(void *arg);
void					take_forks_and_eat(t_philosopher *phil);
void					cleanup_and_exit(t_philosopher *phil, int exit_code);
void					cleanup_threads(t_data *data);

#endif