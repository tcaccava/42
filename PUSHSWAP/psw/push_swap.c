/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 13:49:20 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 18:00:17 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static t_stack	*handle_single_argument(char *arg)
{
	t_stack	*stack;
	char	**numbers;
	int		i;

	i = 0;
	stack = NULL;
	numbers = ft_split(arg, ' ');
	if (!numbers)
		write_error();
	while (numbers[i])
	{
		if (!is_int(numbers[i]) || !is_valid_int(numbers[i]))
			write_error();
		add_to_stack(&stack, ft_atoi(numbers[i]));
		i++;
	}
	i = 0;
	while (numbers[i] != NULL)
	{
		free(numbers[i]);
		i++;
	}
	free(numbers);
	return (stack);
}

static t_stack	*handle_multiple_arguments(int argc, char **argv)
{
	t_stack	*stack;
	int		i;

	stack = NULL;
	i = 1;
	while (i < argc)
	{
		if (!is_int(argv[i]) || !is_valid_int(argv[i]))
			write_error();
		add_to_stack(&stack, ft_atoi(argv[i]));
		i++;
	}
	return (stack);
}

t_stack	*validate_input(int argc, char **argv)
{
	t_stack	*stack;

	stack = NULL;
	if (argc == 2)
		stack = handle_single_argument(argv[1]);
	else if (argc > 2)
	{
		stack = handle_multiple_arguments(argc, argv);
	}
	check_duplicates(stack);
	return (stack);
}

void	free_stack(t_stack **stack)
{
	t_stack	*current;
	t_stack	*next;

	if (!stack || !*stack)
		return ;
	current = *stack;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*stack = NULL;
}

void	validate_input_bonus(int argc, char **argv, t_stack **a)
{
	t_stack	*stack;

	if (argc < 2)
	{
		write(2, "Error\n", 6);
		exit(1);
	}
	if (argc == 2)
		stack = handle_single_argument(argv[1]);
	else
		stack = handle_multiple_arguments(argc, argv);
	check_duplicates(stack);
	*a = stack;
}
