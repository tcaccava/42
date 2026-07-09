/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:14:19 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/06 17:43:51 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	error_call(t_list *lst)
{
	write(2, "Error\n", 6);
	free_lst(lst);
	exit(1);
}

static int	duplicate_check(int n, t_list *lst)
{
	while (lst)
	{
		if (lst->nb == n)
			return (1);
		lst = lst->next;
	}
	return (0);
}

static void	check_digit(char *argv, t_list **lst, int nb, int *y)
{
	nb = ft_atoi(argv, y, *lst);
	if (!duplicate_check(nb, *lst))
		ft_lstpush(lst, ft_lstnew(nb));
	else
		error_call(*lst);
}

void	args_parser(char **argv, t_list **lst, t_flags *flag, int i)
{
	int	y;
	int	nb;

	nb = 0;
	while (argv[i])
	{
		y = 0;
		if (argv[i][0] == 0)
			error_call(*lst);
		while (argv[i][y])
		{
			if (ft_isspace(argv[i][y]))
				y++;
			else if (argv[i][y] == '-' && argv[i][y + 1] == '-')
				flags_parser(flag, &argv[i][y], &y, *lst);
			else if (ft_isdigit(argv[i][y]) || ((argv[i][y] == '-'
					|| argv[i][y] == '+') && ft_isdigit(argv[i][y + 1])))
				check_digit(&argv[i][y], lst, nb, &y);
			else
				error_call(*lst);
		}
		i++;
	}
}
