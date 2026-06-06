/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:58:32 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/05 17:32:41 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*mem;

	if (count == 0 || size == 0)
	{
		mem = malloc(1);
		if (!mem)
			return (NULL);
		return (mem);
	}
	if (size != 0 && count > SIZE_MAX / size)
		return (NULL);
	mem = malloc(count * size);
	if (!mem)
		return (NULL);
	ft_memset(mem, 0, (count * size));
	return (mem);
}
