/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <mde-matt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:38:40 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/21 13:24:09 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

int	dupchecker(int ac, char **av)
{
	int	i;
	int	n;

	i = 1;
	while (i < ac)
	{
		n = i + 1;
		while (n < ac)
		{
			if (ft_atoi(av[i]) == ft_atoi(av[n]))
				return (0);
			n++;
		}
		i++;
	}
	return (1);
}

int	overcheck(char *str)
{
	char	*max;
	int		len;

	if (ft_strlen(str) > 10)
	{
		if ((str[0] != '-' && str[0] != '+') || ft_strlen(str) > 11)
			return (0);
	}
	if (ft_strlen(str) < 10)
		return (1);
	if (str[0] == '-')
		max = "2147483648";
	else
		max = "2147483647";
	if (str[0] == '-' || str[0] == '+')
		str++;
	len = ft_strlen(str);
	if (len < 10)
		return (1);
	if (ft_strncmp(str, max, 10) <= 0)
		return (1);
	else
		return (0);
}

int	numchecker(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (overcheck(str) == 0)
		return (0);
	return (1);
}

t_list	*stack_parser(int ac, char **av)
{
	t_list	*val;
	t_list	*stack;
	int		i;
	int		n;

	i = 1;
	stack = NULL;
	n = dupchecker(ac, av);
	while (i < ac)
	{
		if (numchecker(av[i]) == 0 || n == 0)
		{
			ft_printf("%s\n", "Error");
			return (NULL);
		}
		val = malloc(sizeof(t_list));
		if (!val)
			return (NULL);
		ft_memset(val, 0, sizeof(t_list));
		val->content = (void *)(long)ft_atoi(av[i]);
		ft_lstadd_back(&stack, val);
		i++;
	}
	return (stack);
}
