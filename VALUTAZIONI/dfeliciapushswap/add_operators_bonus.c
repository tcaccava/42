/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_operators_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <mde-matt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 17:22:08 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/17 17:47:53 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_checker_bonus.h"

void	ra(t_list **stack)
{
	t_list	*temp;

	if (!*stack)
		return ;
	temp = *stack;
	*stack = (*stack)->next;
	temp->next = NULL;
	ft_lstadd_back(stack, temp);
}

void	rb(t_list **stack)
{
	t_list	*temp;

	if (!*stack)
		return ;
	temp = *stack;
	*stack = (*stack)->next;
	temp->next = NULL;
	ft_lstadd_back(stack, temp);
}

void	rr(t_list **stacka, t_list **stackb)
{
	ra(stacka);
	rb(stackb);
}

void	rra(t_list **stack)
{
	t_list	*temp;
	t_list	*templast;

	if (!*stack || !(*stack)->next)
		return ;
	templast = *stack;
	while (templast->next->next != NULL)
		templast = templast->next;
	temp = templast->next;
	templast->next = NULL;
	ft_lstadd_front(stack, temp);
}

void	rrb(t_list **stack)
{
	t_list	*temp;
	t_list	*templast;

	if (!*stack || !(*stack)->next)
		return ;
	templast = *stack;
	while (templast->next->next != NULL)
		templast = templast->next;
	temp = templast->next;
	templast->next = NULL;
	ft_lstadd_front(stack, temp);
}
