/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_checker_bonus.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <mde-matt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 17:10:00 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/21 13:11:02 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CHECKER_BONUS_H
# define FT_CHECKER_BONUS_H

# include <stdlib.h>
# include <unistd.h>
# include "libft/libft.h"
# include "printf/libftprintf.h"
# include "get_next_line/get_next_line.h"

int		dupchecker(int ac, char **av);
int		overcheck(char *str);
int		numchecker(char *str);
int		sortchecker(t_list *stack);
t_list	*stack_parser(int ac, char **av);
void	sa(t_list **stack);
void	sb(t_list **stack);
void	ss(t_list **stacka, t_list **stackb);
void	pb(t_list **stacka, t_list **stackb);
void	pa(t_list **stacka, t_list **stackb);
void	ra(t_list **stack);
void	rb(t_list **stack);
void	rr(t_list **stacka, t_list **stackb);
void	rra(t_list **stack);
void	rrb(t_list **stack);
void	rrr(t_list **stacka, t_list **stackb);
void	mimic(t_list **stack_a, t_list **stack_b, char *line);
void	stackfree(t_list *stack);

#endif
