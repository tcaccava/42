/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:13:17 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/28 16:57:50 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	*mem_int_allocate(void)
{
	int	*value;

	value = malloc(sizeof(int));
	if (value == NULL)
	{
		ft_printf("Error, no allocation!\n");
		return (NULL);
	}
	return (value);
}

void	free_split(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
