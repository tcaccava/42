/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:31:33 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/07 14:52:14 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	choose_algorithm(t_list **a, t_list **b, t_count *count)
{
	count->adaptive_called = TRUE;
	if (count->disorder < 0.2)
	{
		simple_min_max_extraction_method(a, b, count);
		count->strategy = ft_strdup("Adaptive / O(n²)");
	}
	else if (count->disorder >= 0.2 && count->disorder < 0.5)
	{
		chunk_sort(a, b, count);
		count->strategy = ft_strdup("Adaptive / O(n√n)");
	}
	else
	{
		radix_sort(a, b, count);
		count->strategy = ft_strdup("Adaptive / O(n log n)");
	}
}

static void	dispatcher(
	t_list **lst_a,
	t_list **lst_b,
	t_count *count,
	t_flags *flag)
{
	if (parsing_size(lst_a, lst_b, ft_lstsize(*lst_a), count))
		count->strategy = ft_strdup("Adaptive");
	if (!is_ordered(*lst_a))
	{
		if (flag->simple == 1 && !flag->complex_s
			&& !flag->medium && !flag->adaptive)
			simple_min_max_extraction_method(lst_a, lst_b, count);
		else if (flag->medium == 1 && !flag->complex_s
			&& !flag->simple && !flag->adaptive)
			chunk_sort(lst_a, lst_b, count);
		else if (flag->complex_s == 1 && !flag->medium
			&& !flag->simple && !flag->adaptive)
			radix_sort(lst_a, lst_b, count);
		else if ((flag->adaptive == 1 || flag->adaptive == 0) && !flag->medium
			&& !flag->complex_s && !flag->simple)
			choose_algorithm(lst_a, lst_b, count);
		else
			error_call(*lst_a);
	}
}

static void	disorder(t_list *lst, t_count *count)
{
	t_list	*j;
	int		mistakes;
	int		total_pairs;

	mistakes = 0;
	total_pairs = 0;
	while (lst)
	{
		j = lst->next;
		while (j)
		{
			total_pairs++;
			if (lst->nb > j->nb)
				mistakes++;
			j = j->next;
		}
		lst = lst->next;
	}
	count->disorder = (float) mistakes / (float) total_pairs;
}

static void	has_bench_been_called(t_flags *flag, t_count *count)
{
	if (flag->bench == 1)
		print_stats_on_stderr(count);
	if (flag->bench == 0)
		free(count->strategy);
}

int	main(int argc, char **argv)
{
	t_list	*lst_a;
	t_list	*lst_b;
	t_flags	flag;
	t_count	count;

	lst_a = NULL;
	lst_b = NULL;
	flags_initializer(&flag);
	count_initializer(&count);
	if (argc == 1)
		error_call(lst_a);
	else
	{
		args_parser(argv, &lst_a, &flag, 1);
		if (!lst_a)
			error_call(lst_a);
	}
	index_list(lst_a);
	disorder(lst_a, &count);
	if (!is_ordered(lst_a))
		dispatcher(&lst_a, &lst_b, &count, &flag);
	else
		count.strategy = ft_strdup("");
	has_bench_been_called(&flag, &count);
	free_lst(lst_a);
}
