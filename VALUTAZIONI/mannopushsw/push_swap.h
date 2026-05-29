/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmanno <fmanno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:31:58 by fmanno            #+#    #+#             */
/*   Updated: 2025/08/26 16:52:14 by fmanno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft/ft_printf/ft_printf.h"
# include "libft/libft.h"
# include <stdlib.h>

typedef struct s_list
{
	int				value;
	int				index;
	struct s_list	*next;
}					t_list;

// Utils
void	free_stack(t_list **stack);
void	free_array(char **str);
void	handle_error(char *msg);

// Sorting utils
int		is_sorted(t_list **stack);
void	handle_min_first(t_list **a, int next_min);
void	handle_nextmin_first(t_list **a, int min);
void	handle_max_first(t_list **a, int min);

// Indexing utils
void	assign_index(t_list **stack);
int		get_min(t_list **stack, int exclude);
t_list	*get_next_min(t_list **stack);
int		get_distance_to_index(t_list **stack, int index);
void	move_to_top(t_list **stack, int distance);

// Algorithm utils
void	radix_sort(t_list **stack_a, t_list **stack_b);
int		get_bit_length(t_list **stack);

// Input validation
int		ft_isnum(char *s);
int		check_duplicate(int n, char **av, int i);
void	check_args(int argc, char **argv);

// Small stack's functions
void	sort_small_stack(t_list **stack_a, t_list **stack_b);
void	move_min_top(t_list **a, int exclude);
void	sort_three(t_list **a);
void	sort_four(t_list **a, t_list **b);
void	sort_five(t_list **a, t_list **b);

// Lists utils
t_list	*ft_lstnew(int value);
void	ft_lstadd_front(t_list **lst, t_list *new);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **stack, t_list *new);
int		ft_lstsize(t_list *node);

// Operations' functions
int		swap(t_list **stack);
int		push(t_list **stack_to, t_list **stack_from);
int		rotate(t_list **stack);
int		reverse_rotate(t_list **stack);

int		sa(t_list **stack_a);
int		sb(t_list **stack_b);
int		ss(t_list **stack_a, t_list **stack_b);
int		pa(t_list **stack_a, t_list **stack_b);
int		pb(t_list **stack_a, t_list **stack_b);
int		ra(t_list **stack_a);
int		rb(t_list **stack_b);
int		rr(t_list **stack_a, t_list **stack_b);
int		rra(t_list **stack_a);
int		rrb(t_list **stack_b);
int		rrr(t_list **stack_a, t_list **stack_b);

#endif
