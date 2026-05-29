/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdonato <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 12:29:11 by tdonato           #+#    #+#             */
/*   Updated: 2025/02/18 12:29:13 by tdonato          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>      // printf
#include <stdlib.h>     // malloc, free
#include <string.h>     // memset
#include <unistd.h>     // write, usleep
#include <sys/time.h>   // gettimeofday
#include <pthread.h>    // pthread_create, pthread_detach, pthread_join, 
						// pthread_mutex_init, pthread_mutex_destroy, 
						// pthread_mutex_lock, pthread_mutex_unlock

typedef struct s_data
{
	int				philo_nb;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;

	pthread_mutex_t	*forks;

	int				someone_died;
	pthread_mutex_t	someone_died_mutex;

	pthread_mutex_t	writing_mutex;

	int				start;
}	t_data;

typedef struct s_philo
{
	int				id;
	t_data			*info;

	pthread_t		thread;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;

	int				eating;
	pthread_mutex_t	eating_mutex;
	int				last_meal_time;

	int				times_eaten;
	pthread_mutex_t	times_eaten_mutex;
}	t_philo;

int		ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);
void	wrong_args(void);
int		get_input(int argc, char **argv, t_data *info, int valid);
void	terminate_program(t_philo *philosophers, t_data *info);
int		c_time(void);
void	ft_usleep(size_t milliseconds);
void	*routine(void *arg);
void	*observer(t_philo *philosophers, t_data *info);
int		print_nl(int id, char *action, t_data *info);
int		unlock_forks(t_philo *philosopher, int exit_code);

/*
void* deposit(void *amount)
{
	pthread_mutex_lock(&mutex);

	// work

	pthread_mutex_unlock(&mutex);
}

int main()
{
	pthread_mutex_t mutex;

	pthread_t thread1;
	pthread_t thread2;

	pthread_mutex_init(&mutex, NULL);

	pthread_create(&thread1, NULL, deposit, (void*) &deposit1);
	pthread_create(&thread2, NULL, deposit, (void*) &deposit2);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	pthread_mutex_destroy(&mutex);
}
*/
