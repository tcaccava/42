/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:49:40 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/29 03:33:01 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "push_swap.h"

int	duplic_check(t_list *stack, int value)
{
	while (stack)
	{
		if (stack->content == value)
			return (1);
		stack = stack->next;
	}
	return (0);
}

/*Adds a number to the stack after performing any necessary checks.*/
int	add_number_stack(t_list **stack, char *str)
{
	int		value;

	if (!valid_parameter(str))
		return (0);
	value = ft_atoi(str);
	if (duplic_check(*stack, value))
		return (0);
	ft_lstadd_back(stack, ft_lstnew(value));
	return (1);
}

/*Handles the case where the user enters numbers
separated by spaces in quotes*/
int	handle_single_arg(char *arg, t_list **stack_a)
{
	char	**number;
	int		i;

	number = ft_split(arg, ' ');
	if (!number)
		return (0);
	i = 0;
	while (number[i])
	{
		if (!add_number_stack(stack_a, number[i]))
		{
			ft_printf("Error\n");
			free_split(number);
			return (0);
		}
		i++;
	}
	free_split(number);
	return (1);
}

/*Handles the main input of the program.*/
int	handle_arg(int ac, char **av, t_list **stack_a)
{
	int	i;

	i = 1;
	if (ac < 2)
	{
		return (0);
	}
	while (av[i])
	{
		if (!handle_single_arg(av[i], stack_a))
		{
			ft_lstclear(stack_a);
			return (0);
		}
		i++;
	}
	return (1);
}
