/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <mde-matt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 17:11:17 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/21 13:10:27 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_checker_bonus.h"

void	stackfree(t_list *stack)
{
	t_list	*next;

	while (stack)
	{
		next = stack->next;
		free(stack);
		stack = next;
	}
}

void	mimic(t_list **stack_a, t_list **stack_b, char *line)
{
	if (ft_strncmp (line, "rrr", 3) == 0)
		rrr(stack_a, stack_b);
	else if (ft_strncmp (line, "rra", 3) == 0)
		rra(stack_a);
	else if (ft_strncmp (line, "rrb", 3) == 0)
		rrb(stack_b);
	else if (ft_strncmp (line, "rr", 2) == 0)
		rr(stack_a, stack_b);
	else if (ft_strncmp (line, "ra", 2) == 0)
		ra(stack_a);
	else if (ft_strncmp (line, "rb", 2) == 0)
		rb(stack_b);
	else if (ft_strncmp(line, "ss", 2) == 0)
		ss(stack_a, stack_b);
	else if (ft_strncmp(line, "sa", 2) == 0)
		sa(stack_a);
	else if (ft_strncmp(line, "sb", 2) == 0)
		sb(stack_b);
	else if (ft_strncmp (line, "pa", 2) == 0)
		pa(stack_a, stack_b);
	else if (ft_strncmp (line, "pb", 2) == 0)
		pb(stack_a, stack_b);
	return ;
}

int	main(int ac, char **av)
{
	t_list	*stack_a;
	t_list	*stack_b;
	char	*line;

	stack_b = NULL;
	if (ac >= 2)
		stack_a = stack_parser(ac, av);
	else
		return (0);
	if (!stack_a)
		return (0);
	line = get_next_line(0);
	while (line)
	{
		mimic(&stack_a, &stack_b, line);
		free (line);
		line = get_next_line(0);
	}
	if (sortchecker(stack_a) == 0 && !stack_b)
		ft_printf("OK\n");
	else
		ft_printf("KO\n");
	stackfree(stack_a);
	stackfree(stack_b);
	return (0);
}
