/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:53:05 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 14:53:09 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <curses.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef struct s_philodata
{
	int						i;
	int						philo_nb;
	pthread_mutex_t			*forks;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						must_eat;
	int						simulation_running;
	pthread_mutex_t			simulation_mutex;
	long long				simulation_start;
	pthread_t				*threads;
	pthread_t				monitor_thread;
	struct s_philosopher	*philosophers;
}							t_philodata;

typedef struct s_philosopher
{
	int						id;
	int						fork_left;
	int						fork_right;
	pid_t					pid;
	long long				last_meal_time;
	int						meals_eaten;
	struct s_philodata		*philo;
}							t_philo;

void						print_status(t_philodata *data, int id,
								char *message);
int							init_philo(t_philo *philosophers,
								t_philodata *philo_data);
int							init_data_philo(t_philodata *philo_data, int ac,
								char **av);
void						ft_sleep(long long duration_ms);
long long					get_time_ms(void);
int							check_simulation_status(t_philodata *data);
void						*monitor_philosophers(void *arg);
void						*philosopher_routine(void *arg);
int							begin_simulation(t_philo *philosophers,
								t_philodata *philo_data);
int							init_philo(t_philo *philosophers,
								t_philodata *philo_data);
int							init_data_philo(t_philodata *philo_data, int ac,
								char **av);
void						ft_sleep(long long duration_ms);
int							all_philosophers_eat_enough(t_philodata *data);
int							handle_single_philo(t_philodata *data,
								t_philo *philo, int first_fork);
void						determine_forks_order(t_philo *philo,
								int *first_fork, int *second_fork);
int							ft_atoi(const char *str);
int							check_philosopher_death_mutex(t_philodata *data,
								int i);
int							allocate_resources(t_philodata **philo_data,
								t_philo **philosophers, int ac, char **av);
void						cleanup_resources(t_philodata *philo_data,
								t_philo *philosophers);
int							is_valid_number(char *str);

#endif
