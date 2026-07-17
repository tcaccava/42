/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:02:51 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/07/14 18:45:09 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_request
{
	int		coder_id;
	long	deadline;
	long	sended_time;
}	t_request;

typedef struct s_heap
{
	t_request	*requets;
	int			size;
}	t_heap;

typedef struct s_coder
{
	int				id;
	int				has_dungles;
	int				compile_dones;
	long			last_compile_init_time;
	long			last_compile_time;
	pthread_cond_t	cond;
}	t_coder;

typedef struct s_dungle
{
	int		id;
	long	last_used;
	int		is_in_use;
}	t_dungle;

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_global_data
{
	int				number_of_coders;
	int				number_of_compiles_required;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	long			dongle_cooldown;
	long			simulation_start;
	t_coder			*coders;
	t_dungle		*dungles;
	t_scheduler		scheduler;
	pthread_mutex_t	mutex;
	pthread_mutex_t	log_mutex;
	t_heap			*heap;
	int				stop_simulation;
	pthread_t		*threads;
}	t_global_data;

typedef struct s_coder_args
{
	int				coder_id;
	t_global_data	*global_data;
}	t_coder_args;

int		ft_safe_atoi(const char *s, int *out);
int		ft_safe_atol(const char *s, long *out);
int		check_args(int argc, char **argv, t_global_data *data);
int		error_exit(t_global_data *data, const char *msg);

long	get_now_time_ms(void);

void	*coder_routine(void *args);
void	*monitor_runtime(void *args);

int		push_to_heap(t_heap *heap, t_request *request,
			t_scheduler scheduler);
int		heap_pop(t_heap *heap, t_request *out,
			t_scheduler scheduler);

void	free_all(t_global_data *data);
void	ft_sleep_ms(long ms);

void	log_event(t_global_data *data, long timestamp,
			int coder_id, const char *msg);

int		dongles_available(t_global_data *g,
			int left_id, int right_id);
int		all_coders_done(t_global_data *g);
int		coder_time_expired(t_global_data *g);

void	check_expired(t_global_data *g, int *expired);

#endif