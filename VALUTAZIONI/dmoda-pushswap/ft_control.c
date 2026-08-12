/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_control.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 17:27:12 by dmoda             #+#    #+#             */
/*   Updated: 2026/08/11 15:01:36 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

int	ft_is_number(char *num)
{
	int	i;

	i = 0;
	if (num[i] == '+' || num[i] == '-')
		i++;
	if (num[i] >= '0' && num[i] <= '9')
	{
		while (num[i])
		{
			if (num[i] >= '0' && num[i] <= '9')
				i++;
			else
				return (0);
		}
	}
	else
	{
		return (0);
	}
	if (ft_is_overflow(num))
		return (0);
	return (1);
}

int	ft_no_duplicate(int argc, char **a)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		if (ft_is_number(a[i]))
		{
			j = i + 1;
			while (j < argc)
			{
				if (ft_is_number(a[j]) && ft_atoi(a[i]) == ft_atoi(a[j]))
					return (0);
				j++;
			}
		}
		i++;
	}
	return (1);
}

int	ft_check_flag(char *str)
{
	static int	flags[5];
	int			i;

	if (!ft_strcmp(str, "--adaptive"))
		flags[0]++;
	else if (!ft_strcmp(str, "--simple"))
		flags[1]++;
	else if (!ft_strcmp(str, "--medium"))
		flags[2]++;
	else if (!ft_strcmp(str, "--complex"))
		flags[3]++;
	else if (!ft_strcmp(str, "--bench"))
		flags[4]++;
	else if (ft_strcmp(str, "print"))
		return (-1);
	i = flags[0] + flags[1] + flags[2] + flags[3];
	if (i > 1 || flags[4] > 1)
		return (-1);
	i = 0;
	while (i++ < 4)
		if (flags[i - 1])
			return (i - 1);
	return (4);
}

int	ft_condition(t_dlist **a, t_dlist **b)
{
	return (((*b)->cont <= (*a)->cont && ((*b)->cont >= (*a)->prev->cont
				|| (*a)->prev->cont >= (*a)->cont))
		|| ((*b)->cont >= (*a)->prev->cont
			&& (*a)->cont <= (*a)->prev->cont));
}

int	ft_is_overflow(char *num)
{
	int		i;
	int		neg;
	char	*limit;

	i = 0;
	neg = 0;
	if (num[i] == '+' || num[i] == '-')
	{
		if (num[i] == '-')
			neg = 1;
		i++;
	}
	if (neg)
		limit = "2147483648";
	else
		limit = "2147483647";
	if (ft_strlen(num + i) > ft_strlen(limit))
		return (1);
	if (ft_strlen(num + i) == ft_strlen(limit) && ft_strcmp(num + i, limit) > 0)
		return (1);
	return (0);
}
