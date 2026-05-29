/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:45:08 by lluciani          #+#    #+#             */
/*   Updated: 2025/06/23 15:50:52 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo	t_philo;
typedef struct s_table	t_table;

typedef struct s_mutex
{
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_log;
	pthread_mutex_t		death_lock;
	pthread_mutex_t		meal_lock;
	pthread_mutex_t		meals_eaten_lock;
}						t_mutex;

typedef struct s_table
{
	int					n_philo;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				start_time;
	long				n_to_eat;
	int					someone_die;
	t_mutex				mutex;

	t_philo				*philo;
}						t_table;

typedef struct s_philo
{
	int					id;
	long				meals_eaten;
	long				last_meal;
	int					n_to_eat;
	int					right_fork;
	int					left_fork;
	int					eating;
	t_table				*table;
	pthread_t			thread;

}						t_philo;

long					ft_atol(char *s);
int						check_input(char **av);
void					init_table(t_table *table, char **av);
void					philo_init(t_table *table);
void					print_log(t_philo *philo, char *str);
void					*monitoring_status(void *arg);
int						create_thread(t_table *table);
int						is_dead(t_philo *philo);
void					philo_eat(t_philo *philo);
void					philo_think(t_philo *philo);
void					philo_sleep(t_philo *philo);
void					*philo_routine(void *arg);
void					*safe_malloc(size_t size);
long					get_current_time(void);
size_t					custom_usleep(size_t mls);
void					free_all(t_table *table);
int						check_all_eat(t_table *table);
int						ft_numstr(char **s);

void					init_mutex(t_table *table);
void					init_forks(t_table *table);
int						unlock_all_mutex(t_philo *philo);
int						is_one_philo(t_table *table);
int						all_philo_eat(t_table *table);
int						philosophers_died(t_table *table);

#endif
