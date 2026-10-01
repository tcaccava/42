/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 03:57:15 by sara              #+#    #+#             */
/*   Updated: 2026/07/27 11:44:51 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	print_ops_line1(t_count *count)
{
	write(2, "[bench] sa: ", 12);
	ft_putnbr_fd(count->sa, 2);
	write(2, " sb: ", 5);
	ft_putnbr_fd(count->sb, 2);
	write(2, " ss: ", 5);
	ft_putnbr_fd(count->ss, 2);
	write(2, " pa: ", 5);
	ft_putnbr_fd(count->pa, 2);
	write(2, " pb: ", 5);
	ft_putnbr_fd(count->pb, 2);
	write(2, "\n", 1);
}

void	print_ops_line2(t_count *count)
{
	write(2, "[bench] ra: ", 12);
	ft_putnbr_fd(count->ra, 2);
	write(2, " rb: ", 5);
	ft_putnbr_fd(count->rb, 2);
	write(2, " rr: ", 5);
	ft_putnbr_fd(count->rr, 2);
	write(2, " rra: ", 6);
	ft_putnbr_fd(count->rra, 2);
	write(2, " rrb: ", 6);
	ft_putnbr_fd(count->rrb, 2);
	write(2, " rrr: ", 6);
	ft_putnbr_fd(count->rrr, 2);
	write(2, "\n", 1);
}
