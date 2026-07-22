/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 16:33:08 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/13 23:32:21 by dfelicia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

void	sa(t_list **stack, int check, t_ops *counter)
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
	if (check == 1)
	{
		counter->sa++;
		ft_printf("sa\n");
	}
}

void	sb(t_list **stack, int check, t_ops *counter)
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
	if (check == 1)
	{
		ft_printf("sb\n");
		counter->sb++;
	}
}

void	ss(t_list **stacka, t_list **stackb, t_ops *counter)
{
	counter->ss++;
	sa(stacka, 0, counter);
	sb(stackb, 0, counter);
	ft_printf("ss\n");
}

void	pb(t_list **stacka, t_list **stackb, t_ops *counter)
{
	t_list	*temp;

	if (!*stacka)
		return ;
	temp = *stacka;
	*stacka = (*stacka)->next;
	temp->next = NULL;
	ft_lstadd_front(stackb, temp);
	counter->pb++;
	ft_printf("pb\n");
}

void	pa(t_list **stacka, t_list **stackb, t_ops *counter)
{
	t_list	*temp;

	if (!*stackb)
		return ;
	temp = *stackb;
	*stackb = (*stackb)->next;
	temp->next = NULL;
	ft_lstadd_front(stacka, temp);
	ft_printf("pa\n");
	counter->pa++;
}
