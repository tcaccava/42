/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bench.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoda <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 14:18:10 by dmoda             #+#    #+#             */
/*   Updated: 2026/08/11 15:03:41 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

int	ft_bench_sp(int mode, char c)
{
	static int	b[5];

	if (c == '\0')
	{
		ft_printf(2, "[bench] sa: %d sb: %d ss: %d pa: %d pb: %d\n",
			b[0], b[1], b[2], b[3], b[4]);
	}
	if (mode == 1)
	{
		if (c == 'a')
			b[0]++;
		else if (c == 'b')
			b[1]++;
		else if (c == 's')
			b[2]++;
	}
	else if (mode == 0)
	{
		if (c == 'a')
			b[3]++;
		else if (c == 'b')
			b[4]++;
	}
	return (b[0] + b[1] + b[2] + b[3] + b[4]);
}

int	ft_bench_r(int mode, char c)
{
	static int	b[6];

	if (c == '\0')
		ft_printf(2, "[bench] ra: %d rb: %d rr: %d rra: %d rrb: %d rrr: %d\n",
			b[0], b[1], b[2], b[3], b[4], b[5]);
	if (mode == 1)
	{
		if (c == 'a')
			b[0]++;
		else if (c == 'b')
			b[1]++;
		else if (c == 'r')
			b[2]++;
	}
	else if (mode == 0)
	{
		if (c == 'a')
			b[3]++;
		else if (c == 'b')
			b[4]++;
		else if (c == 'r')
			b[5]++;
	}
	return (b[0] + b[1] + b[2] + b[3] + b[4] + b[5]);
}

int	ft_is_bench(char *s)
{
	return (s && !ft_strcmp(s, "--bench"));
}

int	ft_print_bench(float d, char *s1, char *s2, int len)
{
	int	i;

	if (!ft_is_bench(s1) && !ft_is_bench(s2))
		return (1);
	ft_printf(2, "[bench] disorder: %f%%\n", d * 100);
	i = ft_check_flag("print");
	if (i == 0 || i == 4)
	{
		if (d <= 0.25 && len <= 10)
			ft_printf(2, "[bench] strategy: Adaptive / O(n²)\n");
		else if ((d <= 0.3 || d >= 0.8) && len <= 80)
			ft_printf(2, "[bench] strategy: Adaptive / O(n√n)\n");
		else
			ft_printf(2, "[bench] strategy: Adaptive / O(nlogn)\n");
	}
	else if (i == 1)
		ft_printf(2, "[bench] strategy: Simple / O(n²)\n");
	else if (i == 2)
		ft_printf(2, "[bench] strategy: Medium / O(n√n)\n");
	else if (i == 3)
		ft_printf(2, "[bench] strategy: Complex / O(nlogn)\n");
	i = ft_bench_sp(2, 'f') + ft_bench_r(2, 'f');
	ft_printf(2, "[bench] total_ops: %d\n", i);
	ft_bench_sp(2, '\0');
	return (0 * ft_bench_r(2, '\0') + 1);
}
