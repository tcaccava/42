/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 16:03:16 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/06 16:08:02 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_putnbr_moves(int n)
{
	if (n < 10)
	{
		write (2, &"0123456789"[n % 10], 1);
		return ;
	}
	ft_putnbr_moves(n / 10);
	write (2, &"0123456789"[n % 10], 1);
	return ;
}

static void	first_line(t_count *moves)
{
	float	var_1;
	float	var_2;

	write (2, "[bench] disorder:  ", 19);
	if (moves->disorder == 0.1000)
	{
		write (2, "100.00%\n", 8);
		return ;
	}
	var_1 = moves->disorder * 100;
	var_2 = (var_1 * 100) - ((int)var_1 * 100);
	ft_putnbr_moves((int) var_1);
	write (2, ".", 1);
	ft_putnbr_moves((int) var_2);
	if ((int) var_2 == 0)
		write (2, "0", 1);
	write (2, "%\n", 2);
}

static void	second_third_and_fourth_lines(t_count *moves)
{
	int	len;

	len = ft_strlen(moves->strategy);
	write (2, "[bench] strategy:  ", 19);
	write (2, moves->strategy, len);
	free(moves->strategy);
	write (2, "\n", 1);
	write (2, "[bench] total_ops:  ", 20);
	ft_putnbr_moves(moves->total);
	write (2, "\n", 1);
	write (2, "[bench] ", 8);
	write (2, "sa:  ", 5);
	ft_putnbr_moves(moves->sa);
	write (2, "  sb:  ", 7);
	ft_putnbr_moves(moves->sb);
	write (2, "  ss:  ", 7);
	ft_putnbr_moves(moves->ss);
	write (2, "  pa:  ", 7);
	ft_putnbr_moves(moves->pa);
	write (2, "  pb:  ", 7);
	ft_putnbr_moves(moves->pb);
	write (2, "\n", 1);
}

static void	last_line(t_count *moves)
{
	write (2, "[bench] ", 8);
	write (2, "ra:  ", 5);
	ft_putnbr_moves(moves->ra);
	write (2, "  rb:  ", 7);
	ft_putnbr_moves(moves->rb);
	write (2, "  rr:  ", 7);
	ft_putnbr_moves(moves->rr);
	write (2, "  rra:  ", 8);
	ft_putnbr_moves(moves->rra);
	write (2, "  rrb:  ", 8);
	ft_putnbr_moves(moves->rrb);
	write (2, "  rrr:  ", 8);
	ft_putnbr_moves(moves->rrr);
	write (2, "\n", 1);
}

void	print_stats_on_stderr(t_count *moves)
{
	first_line(moves);
	second_third_and_fourth_lines(moves);
	last_line(moves);
}
