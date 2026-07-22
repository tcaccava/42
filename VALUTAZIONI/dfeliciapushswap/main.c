/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <mde-matt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/17 14:42:21 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/21 13:16:41 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

int	flagcheck(char *flag)
{
	if (ft_strncmp(flag, "--simple", 9) == 0)
		return (1);
	if (ft_strncmp(flag, "--medium", 9) == 0)
		return (2);
	if (ft_strncmp(flag, "--complex", 10) == 0)
		return (3);
	if (ft_strncmp(flag, "--adaptive", 11) == 0)
		return (4);
	ft_printf("%s\n", "Error");
	return (0);
}

void	solverpick(t_list **stack_a, t_list **stack_b,
		int check, t_ops *counter)
{
	if (check == 1)
		simplesolver(stack_a, stack_b, counter);
	if (check == 2)
		mediumsolver(stack_a, stack_b, counter);
	if (check == 3)
		complexsolver(stack_a, stack_b, counter);
	if (check == 4)
		adaptivesolver(stack_a, stack_b, counter);
}

void	sorter(char **av, int ac, int check, int bench)
{
	t_list	*stack_a;
	t_list	*stack_b;
	t_ops	counter;
	int		disorder;

	disorder = 0;
	ft_memset(&counter, 0, sizeof(t_ops));
	stack_b = NULL;
	stack_a = stack_parser(ac, av);
	if (!stack_a)
		return ;
	if (sortchecker(stack_a))
	{
		disorder = disordermeter(stack_a);
		if (ac == 3)
			twostack(&stack_a, &counter);
		else if (ac == 4)
			threestack(&stack_a, &counter);
		else if (ac > 4)
			solverpick(&stack_a, &stack_b, check, &counter);
		if (bench == 1)
			benchmaker(check, disorder, &counter);
	}
	stackfree(stack_a);
	stackfree(stack_b);
}

int	main(int ac, char **av)
{
	int		check;
	int		numstart;
	int		bench;

	bench = 0;
	check = 4;
	numstart = 1;
	if (ac < 2)
		return (0);
	if (ac >= 2 && av[1][0] == '-' && av[1][1] == '-')
	{
		if (ft_strncmp(av[1], "--bench", 8) == 0)
		{
			bench = 1;
			numstart++;
		}
		if (numstart < ac && av[numstart][0] == '-' && av[numstart][1] == '-')
		{
			check = flagcheck(av[numstart]);
			numstart++;
		}
	}
	if (check != 0)
		sorter(av + numstart - 1, ac - numstart + 1, check, bench);
	return (0);
}
