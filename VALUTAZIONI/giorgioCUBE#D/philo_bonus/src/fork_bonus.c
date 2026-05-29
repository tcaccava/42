/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:09:07 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/21 13:48:02 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	release_forks(t_philo *philo)
{
	sem_post(philo->table->sems[SEM_FORKS_ID]);
	sem_post(philo->table->sems[SEM_FORKS_ID]);
}
