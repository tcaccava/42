/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:09:29 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 07:13:51 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_int(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < 48 || str[i] > 57)
			return (0);
		i++;
	}
	return (1);
}

void	write_error(void)
{
	write(2, "Error\n", 6);
	exit(1);
}

int	is_valid_int(char *str)
{
	double	num;
	int		sign;
	int		i;

	i = 0;
	sign = 1;
	num = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = sign * -1;
		i++;
	}
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		i++;
		if (num * sign > INT_MAX || num * sign < INT_MIN)
			write_error();
	}
	return (1);
}

void	check_duplicates(t_stack *stack)
{
	t_stack	*current;
	t_stack	*checker;

	current = stack;
	while (current)
	{
		checker = current->next;
		while (checker)
		{
			if (current->value == checker->value)
				write_error();
			checker = checker->next;
		}
		current = current->next;
	}
}

int	is_sorted(t_stack *stack)
{
	if (!stack)
		return (1);
	while (stack->next)
	{
		if (stack->value > stack->next->value)
			return (0);
		stack = stack->next;
	}
	return (1);
}
