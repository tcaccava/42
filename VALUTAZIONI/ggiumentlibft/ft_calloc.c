/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:55:32 by ggiument          #+#    #+#             */
/*   Updated: 2026/06/04 20:55:25 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*array;
	size_t	i;

	i = 0;
	if (count && size > SIZE_MAX / count)
		return (NULL);
	array = malloc(count * size);
	if (array == NULL)
		return (NULL);
	while (i != (count * size))
	{
		array[i] = '\0';
		i++;
	}
	return (array);
}
