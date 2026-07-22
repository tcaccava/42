/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_operators.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:32:38 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/14 01:28:01 by dfelicia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

void	ra(t_list **stack, int check, t_ops *counter)
{
	t_list	*temp;

	if (!*stack)
		return ;
	temp = *stack;
	*stack = (*stack)->next;
	temp->next = NULL;
	ft_lstadd_back(stack, temp);
	if (check == 1)
	{
		ft_printf("ra\n");
		counter->ra++;
	}
}

void	rb(t_list **stack, int check, t_ops *counter)
{
	t_list	*temp;

	if (!*stack)
		return ;
	temp = *stack;
	*stack = (*stack)->next;
	temp->next = NULL;
	ft_lstadd_back(stack, temp);
	if (check == 1)
	{
		ft_printf("rb\n");
		counter->rb++;
	}
}

void	rr(t_list **stacka, t_list **stackb, t_ops *counter)
{
	counter->rr++;
	ra(stacka, 0, counter);
	rb(stackb, 0, counter);
	ft_printf("rr\n");
}

void	rra(t_list **stack, int check, t_ops *counter)
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
	if (check == 1)
	{
		ft_printf("rra\n");
		counter->rra++;
	}
}

void	rrb(t_list **stack, int check, t_ops *counter)
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
	if (check == 1)
	{
		ft_printf("rrb\n");
		counter->rrb++;
	}
}
