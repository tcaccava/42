/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_moves.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:42:03 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/06 17:42:04 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	double_swap(t_list **lst_a, t_list **lst_b, t_count *count)
{
	swap(lst_a, 's', count);
	swap(lst_b, 's', count);
	write(1, "ss\n", 3);
	count->ss += 1;
	count->total += 1;
}

void	double_rotate(t_list **lst_a, t_list **lst_b, t_count *count)
{
	rotate(lst_a, 'r', count);
	rotate(lst_b, 'r', count);
	write(1, "rr\n", 3);
	count->rr += 1;
	count->total += 1;
}

void	double_reverse_rotate(t_list **lst_a, t_list **lst_b, t_count *count)
{
	reverse_rotate(lst_a, 'r', count);
	reverse_rotate(lst_b, 'r', count);
	write(1, "rrr\n", 4);
	count->rrr += 1;
	count->total += 1;
}
