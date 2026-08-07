/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:39 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:25:42 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <sys/time.h>
# include <stddef.h>
# include <limits.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_dongle	t_dongle;
typedef struct s_coder	t_coder;
typedef struct s_sim	t_sim;
typedef struct s_pq		t_pq;

typedef enum e_scheduler
{
	SCHED_INVALID = -1,
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	long			last_used_time;
	int				in_cooldown;
	int				in_use;
	t_pq			*wait_queue;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	int				compiles_done;
	int				is_compiling;
	long			last_compile_time;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_mutex_t	coder_mutex;
	t_sim			*sim;
}	t_coder;

typedef struct s_sim
{
	int				n_coders;
	long			start_time;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				n_compiles_required;
	long			dongle_cooldown;
	t_scheduler		scheduler;
	long			fifo_counter;
	int				sim_stop;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	monitor_mutex;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_sim;

typedef struct s_pq_node
{
	int		coder_id;
	long	priority;
}	t_pq_node;

typedef struct s_pq
{
	t_pq_node	*heap;
	int			size;
	int			capacity;
}	t_pq;

size_t		ft_strlen(char *s);
void		putstr_fd(int fd, char *s);
void		putnbr_fd(int fd, long n);
long		atol_check(char *s);
void		*ft_calloc(size_t n_mem, size_t size);

t_sim		*init_sim(void);

int			init_sim_coders(t_sim *sim);
void		free_all_coders(t_coder *l_coders, int count);
int			check_sim_stop(t_sim *sim);
void		set_sim_stop(t_sim *sim, int n);

int			init_sim_dongles(t_sim *sim);
void		free_all_dongles(t_dongle *l_dongles, int count);
void		release_dongle(t_dongle *dongle, t_sim *sim);
int			acquire_both_dongles(t_coder *coder, t_dongle *dongle);

void		*coder_routine(void *arg);
void		*monitor_routine(void *arg);

t_pq		*pq_create(int capacity);
int			pq_push(t_pq *pq, int coder_id, long priority);
t_pq_node	pq_pop(t_pq *pq);
void		pq_destroy(t_pq *pq);

void		swap_nodes(t_pq_node *a, t_pq_node *b);
int			is_smaller(t_pq_node *a, t_pq_node *b);
long		calculate_priority(t_coder *coder);

void		log_status(t_coder *coder, char *msg);

long		get_current_time_ms(void);
void		custom_usleep(t_coder *coder, long time_to_wait);

t_sim		*validate_input(char **argv);

int			finish_prog(t_sim *sim, char *msg, int exit_code);
#endif
