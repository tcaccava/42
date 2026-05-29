/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 03:41:21 by apintaur          #+#    #+#             */
/*   Updated: 2024/12/16 22:42:44 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*array;
	size_t			total_size;

	if (size && ((nmemb * size) / size) != nmemb)
		return (NULL);
	total_size = nmemb * size;
	array = malloc (total_size);
	if (array != NULL)
		ft_bzero(array, total_size);
	return (((void *)array));
}
