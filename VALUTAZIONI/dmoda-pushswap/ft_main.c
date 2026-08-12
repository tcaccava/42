/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 13:20:45 by dmoda             #+#    #+#             */
/*   Updated: 2026/08/11 14:07:11 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

int	ft_create_list(t_dlist **head_a, int argc, char **argv)
{
	int		i;
	int		flag;

	flag = 0;
	i = 1;
	if (!ft_no_duplicate(argc, argv))
		return (0);
	while (argv[i])
	{
		if (ft_is_number(argv[i]))
		{
			if (!ft_add_node(head_a, ft_new_node(ft_atoi(argv[i]))))
				return (0);
			flag++;
		}
		else if (i >= 3 || ft_check_flag(argv[i]) == -1 || flag)
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_chose_algoritm(char **a, t_dlist **head_a, t_dlist **head_b)
{
	float	disorder;
	int		len;

	len = ft_stack_len(*head_a);
	disorder = ft_disorder(*head_a, len);
	if (!(*head_a) || *head_a == (*head_a)->next || !disorder)
		return (ft_print_bench(disorder, a[1], a[2], len));
	if (!ft_strcmp(a[1], "--simple") || !ft_strcmp(a[2], "--simple"))
		ft_bubble(head_a, head_b);
	else if (!ft_strcmp(a[1], "--medium") || !ft_strcmp(a[2], "--medium"))
		ft_block(head_a, head_b, ft_sqrt(len));
	else if (!ft_strcmp(a[1], "--complex") || !ft_strcmp(a[2], "--complex"))
		ft_quick(head_a, head_b, len);
	else
	{
		if (disorder <= 0.25 && len <= 10)
			ft_bubble(head_a, head_b);
		else if ((disorder <= 0.3 || disorder >= 0.8) && len <= 80)
			ft_block(head_a, head_b, ft_sqrt(len));
		else
			ft_quick(head_a, head_b, len);
	}
	if (!ft_disorder(*head_a, len))
		return (ft_print_bench(disorder, a[1], a[2], len));
	return (0);
}

/*void ft_print(t_dlist **head)
{
	t_dlist *node;
	
	if (!(*head))
		return ;
	node = (*head)->next;
	printf("inizio stack\n%d\n", (*head)->cont);
	while(node != *head)
	{
		printf("%d\n", node->cont);
		node = node->next;
	}
	printf("fine stack\n");
}*/

int	main(int argc, char **argv)
{
	t_dlist	**head_a;
	t_dlist	**head_b;

	if (argc == 1)
		return (0);
	head_a = ft_new_head();
	head_b = ft_new_head();
	if (!head_a || !head_b)
	{
		if (head_a)
			free(head_a);
		if (head_b)
			free(head_b);
		write(2, "Error\n", 6);
		return (0);
	}
	if (!ft_create_list(head_a, argc, argv))
		write(2, "Error\n", 6);
	else if (!ft_chose_algoritm(argv, head_a, head_b))
		write(2, "Error\n", 6);
	ft_free_list(head_b);
	ft_free_list(head_a);
	free(head_a);
	free(head_b);
}
