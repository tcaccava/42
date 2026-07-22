/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_push_swap.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 17:07:23 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/14 18:33:20 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PUSH_SWAP_H
# define FT_PUSH_SWAP_H

# include <unistd.h>
# include <stdlib.h>
# include "libft/libft.h"
# include "printf/libftprintf.h"

typedef struct s_ops
{
	int	sa;
	int	sb;
	int	ss;
	int	pa;
	int	pb;
	int	ra;
	int	rb;
	int	rr;
	int	rra;
	int	rrb;
	int	rrr;
}		t_ops;

int		dupchecker(int ac, char **av);
int		overcheck(char *str);
int		numchecker(char *str);
int		sortchecker(t_list *stack);
t_list	*stack_parser(int ac, char **av);
void	sa(t_list **stack, int check, t_ops *counter);
void	sb(t_list **stack, int check, t_ops *counter);
void	ss(t_list **stacka, t_list **stackb, t_ops *counter);
void	pb(t_list **stacka, t_list **stackb, t_ops *counter);
void	pa(t_list **stacka, t_list **stackb, t_ops *counter);
void	ra(t_list **stack, int check, t_ops *counter);
void	rb(t_list **stack, int check, t_ops *counter);
void	rr(t_list **stacka, t_list **stackb, t_ops *counter);
void	rra(t_list **stack, int check, t_ops *counter);
void	rrb(t_list **stack, int check, t_ops *counter);
void	rrr(t_list **stacka, t_list **stackb, t_ops *counter);
void	twostack(t_list **stack, t_ops *counter);
void	threestack(t_list **stack, t_ops *counter);
void	simplesolver(t_list **stacka, t_list **stackb, t_ops *counter);
void	minraiser(t_list **stacka, int minpointer, t_ops *counter);
int		find_min(t_list *stack);
int		groupdivider(int n);
int		find_max(t_list *stack);
int		min_value(t_list *stack);
int		max_value(t_list *stack);
int		groupassigner(int value, t_list *stack);
int		group_population(t_list *stack, int c);
void	mediumsolver(t_list **stacka, t_list **stackb, t_ops *counter);
void	stackfree(t_list *stack);
t_list	*stackdup(t_list *stack);
void	chunk_pb(t_list **stacka, t_list **stackb, int limit, t_ops *counter);
int		groupfinder(t_list **stacka, t_list *original, int group);
int		count_bigger(t_list *stackb, int limit, int value);
void	solverpick(t_list **stack_a, t_list **stack_b,
			int check, t_ops *counter);
int		flagcheck(char *flag);
int		disordermeter(t_list *stack);
void	strategyused(int check);
int		flagcheck(char *flag);
void	complexsolver(t_list **stacka, t_list **stackb, t_ops *counter);
void	adaptivesolver(t_list **stacla, t_list **stackb, t_ops *counter);
void	totalops(t_ops *counter);
void	opsprinter(t_ops *counter);
void	benchmaker(int check, int disorder, t_ops *counter);
void	sorter(char **av, int ac, int check, int bench);

#endif
