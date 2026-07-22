/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <mde-matt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 17:21:50 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/17 17:47:48 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_checker_bonus.h"

void	sa(t_list **stack)
{
	t_list	*tempa;
	t_list	*tempb;

	if (!*stack || !(*stack)->next)
		return ;
	tempa = *stack;
	tempb = (*stack)->next;
	tempa->next = tempb->next;
	tempb->next = tempa;
	*stack = tempb;
}

void	sb(t_list **stack)
{
	t_list	*tempa;
	t_list	*tempb;

	if (!*stack || !(*stack)->next)
		return ;
	tempa = *stack;
	tempb = (*stack)->next;
	tempa->next = tempb->next;
	tempb->next = tempa;
	*stack = tempb;
}

void	ss(t_list **stacka, t_list **stackb)
{
	sa(stacka);
	sb(stackb);
}

void	pb(t_list **stacka, t_list **stackb)
{
	t_list	*temp;

	if (!*stacka)
		return ;
	temp = *stacka;
	*stacka = (*stacka)->next;
	temp->next = NULL;
	ft_lstadd_front(stackb, temp);
}

void	pa(t_list **stacka, t_list **stackb)
{
	t_list	*temp;

	if (!*stackb)
		return ;
	temp = *stackb;
	*stackb = (*stackb)->next;
	temp->next = NULL;
	ft_lstadd_front(stacka, temp);
}
