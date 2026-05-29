/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kadferna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 12:01:02 by kadferna          #+#    #+#             */
/*   Updated: 2025/05/09 12:01:05 by kadferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

# define MAX_PHILOS 200
# define NOT_GREATER_0 "sleep and eat time must always be greater than zero"

typedef struct s_data
{
	int				num_philos;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	int				max_meals;
	bool			death_flag;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	meal_mutex;
	time_t			start_time;
}					t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	time_t			last_meal_time;
	int				meals_eaten;
	t_data			*data;
}					t_philo;

t_data				*check_init(int argc, char **argv, int *nums,
						t_philo *philos);
int					create_philos_routine(t_philo *philos,
						void *routine(void *), int i);
void				check_forks(t_philo *philo, pthread_mutex_t *first_fork,
						pthread_mutex_t *second_fork);
int					check_philos(t_data *d, t_philo *philos, int *all_ate);
int					check_death_flag(t_data *data, t_philo *philos,
						long time_since_last_meal, int i);
int					pick_forks_and_eat(t_philo *philo,
						pthread_mutex_t *first_fork,
						pthread_mutex_t *second_fork);
void				*ft_memset(void *s, int c, size_t n);
void				print_status(t_philo *philo, char *status);
int					is_valid_number(const char *str);
void				destroy_mutex(t_data *data);
void				ft_usleep(time_t milliseconds, t_data *data);
bool				if_odd(int n);
void				free_data_struct(t_data *data, t_philo *philos);
int					ft_atoi(const char *nptr);
void				cust_error(void);
void				std_error(char *msg);
int					has_letters(char *str);
int					check_numbers(int *nums, int argc, char **argv);
int					checks(int *nums, int argc, char **argv);
int					check_num_values(int *nums);
t_data				*init_data(int *nums);
t_philo				*init_philos(t_data *data);
int					init_forks(t_data *data, t_philo *philos);
time_t				get_time(void);
t_data				*init(int *nums, t_philo **philos);
void				free_philos(t_philo *philos);
t_data				*do_checks_and_parse(t_philo *philos, int *nums);
#endif
