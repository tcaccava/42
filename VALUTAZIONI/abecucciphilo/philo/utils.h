/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abecucci <abecucci@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 19:44:41 by abecucci          #+#    #+#             */
/*   Updated: 2026/03/03 13:37:06 by abecucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/* --- STRUTTURE --- */
typedef struct s_shared	t_shared;
typedef struct s_philo	t_philo;

typedef enum e_bool
{
	False,
	True
}					t_bool;

/* --- FUNZIONI --- */
uint64_t		time_now_ms(void);
void			print_state(t_philo *p, char *state);
void			raise_stop(t_shared *sh);
t_bool			should_stop(t_shared *sh);
t_bool			ft_msleep(t_shared *sh, uint64_t sleep_time_ms);

#endif //UTILS_H