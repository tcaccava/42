/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdecarli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:48:43 by mdecarli          #+#    #+#             */
/*   Updated: 2026/06/02 16:22:22 by mdecarli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n);

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (count != 0 && size > (size_t)-1 / count)
		return (NULL);
	ptr = malloc(count * size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}

/*int	main(void)
{
	int *arr = (int *)ft_calloc(6, sizeof(int));
	printf("ft_calloc %d\n", arr[0]);
	printf("ft_calloc %d\n", arr[5]);
	free(arr);
	printf("--- \n");
}*/
