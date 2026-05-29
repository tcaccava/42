/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:17:02 by abkhefif          #+#    #+#             */
/*   Updated: 2025/03/26 15:21:10 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <fcntl.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_philosopher
{
	int					id;
	int					fork_left;
	int					fork_right;
	pid_t				pid;
	long long			last_meal_time;
	int					meals_eaten;
	struct s_philodata	*philo_data;
}						t_philo;

typedef struct s_philodata
{
	int					i;
	int					philo_nb;
	sem_t				*forks_sem;
	sem_t				*write_sem;
	sem_t				*status_sem;
	sem_t				*meal_sem;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat;
	int					simulation_running;
	long long			start_time;
	t_philo				*philosophers;
	sem_t				**forks;
	sem_t				*simulation_sem;

}						t_philodata;

int						check_simulation_status_bonus(t_philodata *data);
void					init_philo_bonus(t_philodata *philo_data,
							t_philo *philosophers);
int						init_data_philo_bonus(int ac, char **av,
							t_philodata *philo_data);
int						begin_simulation_bonus(t_philo *philosophers,
							t_philodata *philo_data);
void					print_status(t_philodata *data, int id, char *message);
int						begin_simulation_bonus(t_philo *philosophers,
							t_philodata *philo_data);
void					philosopher_routine_bonus(t_philo *philo);
void					sleep_and_think(t_philo *philo);
void					release_forks(t_philo *philo);
void					check_meals_count(t_philo *philo, int meals_count);
void					take_forks(t_philo *philo);
void					handle_single_philo(t_philo *philo);
void					eat_action(t_philo *philo);
long long				get_time_ms(void);
void					ft_sleep(long long ms);
void					cleanup_resources(t_philodata *philo_data);
int						ft_atoi(const char *str);
int						check_philosopher_death(t_philodata *data, int i);
int						init_global_semaphores(t_philodata *philo_data);
int						init_philo_data_basic(int ac, char **av,
							t_philodata *philo_data);
int						init_fork_semaphores(t_philodata *philo_data);
void					cleanup_resources(t_philodata *philo_data);
void					*monitor_philosophers_bonus(void *arg);
char					*ft_itoa(int n);
char					*ft_strcat(char *dest, char *src);
void					*ft_memset(void *b, int c, size_t len);
int						take_forks_bonus(t_philo *philosopher, int left,
							int right);
int						eat_bonus(t_philo *philosopher);
int						sleep_and_think_bonus(t_philo *philosopher);
int						is_valid_number(char *str);
void					cleanup_forks(t_philodata *philo_data);
#endif
