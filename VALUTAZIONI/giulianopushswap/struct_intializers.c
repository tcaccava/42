/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_intializers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:17:14 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/06 17:52:19 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	flags_initializer(t_flags *flag)
{
	flag->bench = FALSE;
	flag->simple = FALSE;
	flag->medium = FALSE;
	flag->complex_s = FALSE;
	flag->adaptive = FALSE;
}

void	count_initializer(t_count *count)
{
	count->pa = 0;
	count->pb = 0;
	count->sa = 0;
	count->sb = 0;
	count->ss = 0;
	count->ra = 0;
	count->rb = 0;
	count->rr = 0;
	count->rra = 0;
	count->rrb = 0;
	count->rrr = 0;
	count->total = 0;
	count->adaptive_called = 0;
}

static void	set_flag_move_index(int *flag, int *y, int offset)
{
	*flag += 1;
	*y += offset;
}

void	flags_parser(t_flags *flag, char *str, int *y, t_list *a)
{
	*y += 2;
	if (str[2] == 'b' && !ft_strncmp(&str[2], "bench", 5))
		set_flag_move_index(&flag->bench, y, 5);
	else if (str[2] == 's' && !ft_strncmp(&str[2], "simple", 6))
		set_flag_move_index(&flag->simple, y, 6);
	else if (str[2] == 'm' && !ft_strncmp(&str[2], "medium", 6))
		set_flag_move_index(&flag->medium, y, 6);
	else if (str[2] == 'c' && !ft_strncmp(&str[2], "complex", 7))
		set_flag_move_index(&flag->complex_s, y, 7);
	else if (str[2] == 'a' && !ft_strncmp(&str[2], "adaptive", 8))
		set_flag_move_index(&flag->adaptive, y, 8);
	else
		error_call(a);
}
