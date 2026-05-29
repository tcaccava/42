/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:04:58 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/29 04:34:29 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <limits.h>

typedef struct s_list
{
	int				content;
	int				index;
	struct s_list	*next;
}	t_list;

/* Linked List Operations */
t_list	*ft_lstnew(int content);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst);
void	ft_lstdelone(t_list *lst);
int		ft_lstsize(t_list *lst);

/* Error Checking */
int		valid_parameter(char *str);
int		*mem_int_allocate(void);
int		add_number_stack(t_list **stack, char *str);
int		handle_arg(int ac, char **av, t_list **stack_a);
int		duplic_check(t_list *stack, int value);
void	free_split(char **arr);

/* Sorting Utilities */
int		is_sorted(t_list *stack);
void	sort_small(t_list **a, t_list **b, int size);
void	sort_three(t_list **a);
void	sort_four(t_list **a, t_list **b);
t_list	*small_node(t_list *a, int *min_pos);
void	move_smallest_b(t_list **a, t_list **b);

/* Sorting Algorithms */
void	radix_sort(t_list **a, t_list **b);
void	assign_index(t_list *a);

/* Operations */
void	sa(t_list **stack_a);
void	sb(t_list **stack_b);
void	ss(t_list **stack_a, t_list **stack_b);
void	pa(t_list **stack_a, t_list **stack_b);
void	pb(t_list **stack_a, t_list **stack_b);
void	ra(t_list **stack_a);
void	rb(t_list **stack_b);
void	rr(t_list **stack_a, t_list **stack_b);
void	rra(t_list **stack_a);
void	rrb(t_list **stack_b);
void	rrr(t_list **stack_a, t_list **stack_b);
void	swappy(t_list **stack);

#endif
