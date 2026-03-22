/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:16:04 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/24 12:14:46 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	execute_instruction(t_stack **a, t_stack **b, char *instruction)
{
	if (ft_strcmp(instruction, "sa\n") == 0)
		silent_sa(*a);
	else if (ft_strcmp(instruction, "sb\n") == 0)
		silent_sb(*b);
	else if (ft_strcmp(instruction, "ss\n") == 0)
		silent_ss(*a, *b);
	else if (ft_strcmp(instruction, "pa\n") == 0)
		pa(a, b);
	else if (ft_strcmp(instruction, "pb\n") == 0)
		silent_pb(a, b);
	else if (ft_strcmp(instruction, "ra\n") == 0)
		silent_ra(a);
	else if (ft_strcmp(instruction, "rb\n") == 0)
		rb(b);
	else if (ft_strcmp(instruction, "rr\n") == 0)
		silent_rr(a, b);
	else if (ft_strcmp(instruction, "rra\n") == 0)
		silent_rra(a);
	else if (ft_strcmp(instruction, "rrb\n") == 0)
		rrb(b);
	else if (ft_strcmp(instruction, "rrr\n") == 0)
		silent_rrr(a, b);
	else
		write(2, "Error\n", 6);
}

void	read_instructions(t_stack **a, t_stack **b)
{
	char	*instruction;

	instruction = get_next_line(0);
	while (instruction)
	{
		execute_instruction(a, b, instruction);
		free(instruction);
		instruction = get_next_line(0);
	}
}

int	main(int argc, char **argv)
{
	t_stack	*a;
	t_stack	*b;

	a = NULL;
	b = NULL;
	if (argc < 2)
		return (0);
	validate_input_bonus(argc, argv, &a);
	read_instructions(&a, &b);
	if (is_sorted(a) && (!b))
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	free_stack(&a);
	free_stack(&b);
	return (0);
}
