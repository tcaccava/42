/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_check2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 23:44:38 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/29 03:38:44 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

//Check if the first character is a sign (+ or -)
static int	check_sign(char c, int *sign)
{
	if (c == '-' || c == '+')
	{
		if (c == '-')
			*sign = -1;
		return (1);
	}
	return (0);
}

//Check if the rest of the string is a valid number
static int	parse_str(char *str, long *numb, int sign)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		*numb = (*numb * 10) + (str[i] - '0');
		if ((*numb * sign) > INT_MAX || (*numb * sign) < INT_MIN)
			return (0);
		i++;
	}
	return (1);
}

/*Final function that checks if the input is a valid number.*/
int	valid_parameter(char *str)
{
	size_t	i;
	long	numb;
	int		sign;

	sign = 1;
	i = 0;
	numb = 0;
	if (str[i] == '\0')
		return (0);
	i += check_sign(str[i], &sign);
	if (!str[i])
		return (0);
	return (parse_str(&str[i], &numb, sign));
}

/*The first two elements of the list are swapped 1 2 3 -> 2 1 3*/
void	swappy(t_list **stack)
{
	t_list	*first;
	t_list	*second;

	if (!stack || !*stack || !((*stack)->next))
		return ;
	first = *stack;
	second = first->next;
	first->next = second->next;
	second->next = first;
	*stack = second;
}
