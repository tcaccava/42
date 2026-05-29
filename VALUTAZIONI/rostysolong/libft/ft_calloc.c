/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:42:29 by rodolhop          #+#    #+#             */
/*   Updated: 2025/01/13 12:45:24 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}

/*
int	main(void)
{
	int		*arr;
	size_t	i;

	// Выделяем память для массива из 5 элементов типа int
	arr = (int *)ft_calloc(5, sizeof(int));
	if (!arr)
	{
		printf("Memory allocation failed\n");
		return (1);
	}

	// Печатаем содержимое массива (должно быть 0, так как память обнулена)
	i = 0;
	while (i < 5)
	{
		printf("%d ", arr[i]);
		i++;
	}
	printf("\n");

	free(arr);
	return (0);
}
*/
