/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:23:20 by sara              #+#    #+#             */
/*   Updated: 2026/07/27 12:08:14 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	print_bench(t_count *count)
{
	print_disorder(count);
	print_strategy(count);
	print_operations(count);
}

void	print_disorder(t_count *count)
{
	write(2, "[bench] disorder: ", 18);
	ft_putfloat_fd(count->disorder * 100, 2);
	write(2, "%\n", 2);
}

void	print_strategy(t_count *count)
{
	write(2, "[bench] strategy: ", 18);
	ft_putstr_fd(count->strategy, 2);
	write(2, " / ", 3);
	ft_putstr_fd(count->complexity, 2);
	write(2, "\n", 1);
}

void	ft_putfloat_fd(float n, int fd)
{
	int	totale;
	int	intera;
	int	decimale;

	totale = (int)(n * 100);
	intera = totale / 100;
	decimale = totale % 100;
	ft_putnbr_fd(intera, fd);
	write(fd, ".", 1);
	if (decimale < 10)
	{
		write(fd, "0", 1);
		ft_putnbr_fd(decimale, fd);
	}
	else
		ft_putnbr_fd(decimale, fd);
}

void	print_operations(t_count *count)
{
	write(2, "[bench] total_ops: ", 19);
	ft_putnbr_fd(count->total, 2);
	write(2, "\n", 1);
	print_ops_line1(count);
	print_ops_line2(count);
}
