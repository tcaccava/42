/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmontana <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:10:30 by dmontana          #+#    #+#             */
/*   Updated: 2026/03/30 14:10:32 by dmontana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules	t_rules;

typedef struct s_philo
{
	int				idx;
	int				id;
	int				meals_eaten;
	int				reported_full;
	int				left_fork_idx;
	int				right_fork_idx;
	long			last_meal_ms;
	pthread_t		thread;
	pthread_mutex_t	meal_mutex;
	t_rules			*rules;
}	t_philo;

// - 1 makes conditions always false, so it means infinite meals
struct s_rules
{
	int				philo_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	long			start_ms;
	int				stop;
	int				finished_count;
	pthread_t		monitor;
	pthread_mutex_t	*forks;
	int				*fork_owner;
	int				*fork_dirty;
	int				*fork_requested;
	t_philo			*philos;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	done_mutex;
	int				core_mutex_inited;
	int				fork_mutex_inited;
	int				meal_mutex_inited;
};

int		parse_args(t_rules *rules, int argc, char **argv);
int		init_rules(t_rules *rules);
int		start_simulation(t_rules *rules);
void	cleanup_rules(t_rules *rules);

long	get_time_ms(void);
void	precise_sleep(t_rules *rules, long sleep_ms);

int		is_stopped(t_rules *rules);
void	set_stop(t_rules *rules, int value);
int		log_status(t_philo *philo, const char *msg, int force);

void	lock_two_forks(t_rules *rules, int a, int b);
void	unlock_two_forks(t_rules *rules, int a, int b);
void	serve_fork_requests(t_philo *philo);
int		owns_both_forks(t_philo *philo);
void	request_missing_forks(t_philo *philo);
void	mark_forks_dirty(t_philo *philo);
void	wait_with_service(t_philo *philo, long wait_ms);
int		register_meal_if_full(t_philo *philo);
int		acquire_both_forks(t_philo *philo);

void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);

int		error_msg(const char *msg);
long	ft_atol_strict(const char *str, int *ok);

#endif
