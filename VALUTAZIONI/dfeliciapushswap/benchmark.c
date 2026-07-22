/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 14:33:01 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/14 18:31:10 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

int	disordermeter(t_list *stack)
{
	t_list	*i;
	t_list	*j;
	int		pairs;
	int		disorder;

	disorder = 0;
	pairs = 0;
	i = stack;
	while (i)
	{
		j = i->next;
		while (j)
		{
			pairs++;
			if ((int)(long)i->content > (int)(long)j->content)
				disorder++;
			j = j->next;
		}
		i = i->next;
	}
	if (!pairs)
		return (0);
	return (disorder * 10000 / pairs);
}

void	strategyused(int check)
{
	char	*str;

	str = NULL;
	if (check == 1)
		str = "Simple Algorithm / O(n^2)\n";
	if (check == 2)
		str = "Medium algorithm / O(n sqrt n)\n";
	if (check == 3)
		str = "Complex algorithm O(n log n)\n";
	if (check == 4)
		str = "Adaptive algorithm.\n";
	if (check == 5)
		str = "Stack already sorted.\n";
	write (2, str, ft_strlen(str));
}

void	totalops(t_ops *counter)
{
	int	total;

	total = counter->sa + counter->sb + counter->ss + counter->pa
		+ counter->pb + counter->ra + counter->rb + counter->rr
		+ counter->rra + counter->rrb + counter->rrr;
	write (2, "total_ops: ", 11);
	ft_putnbr_fd(total, 2);
	write(2, "\n", 1);
}

void	opsprinter(t_ops *counter)
{
	write (2, "sa: ", 4);
	ft_putnbr_fd(counter->sa, 2);
	write (2, " sb: ", 5);
	ft_putnbr_fd(counter->sb, 2);
	write (2, " ss: ", 5);
	ft_putnbr_fd(counter->ss, 2);
	write (2, " pa: ", 5);
	ft_putnbr_fd(counter->pa, 2);
	write (2, " pb: ", 5);
	ft_putnbr_fd(counter->pb, 2);
	write (2, "\n[bench] ra: ", 13);
	ft_putnbr_fd(counter->ra, 2);
	write (2, " rb: ", 5);
	ft_putnbr_fd(counter->rb, 2);
	write (2, " rr: ", 5);
	ft_putnbr_fd(counter->rr, 2);
	write (2, " rra: ", 6);
	ft_putnbr_fd(counter->rra, 2);
	write (2, " rrb: ", 6);
	ft_putnbr_fd(counter->rrb, 2);
	write (2, " rrr: ", 6);
	ft_putnbr_fd(counter->rrr, 2);
	write (2, "\n", 1);
}

void	benchmaker(int check, int disorder, t_ops *counter)
{
	char	*prefix;

	prefix = "[bench] ";
	write (2, prefix, ft_strlen(prefix));
	write (2, "disorder: ", 10);
	ft_putnbr_fd(disorder / 100, 2);
	write (2, ".", 1);
	if ((disorder % 100) < 10)
		write (2, "0", 1);
	ft_putnbr_fd(disorder % 100, 2);
	write (2, "%\n", 2);
	write (2, prefix, ft_strlen(prefix));
	write (2, "strategy: ", 10);
	strategyused(check);
	write (2, prefix, ft_strlen(prefix));
	totalops(counter);
	write (2, prefix, ft_strlen(prefix));
	opsprinter(counter);
}
