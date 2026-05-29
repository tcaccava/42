/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smattei <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:47:24 by smattei           #+#    #+#             */
/*   Updated: 2025/01/21 15:47:27 by smattei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 

void	*ft_calloc(size_t nelem, size_t elsize)
{
	void	*mem;

	if (nelem != 0 && elsize > (SIZE_MAX / nelem))
		return (NULL);
	mem = (void *)malloc(nelem * elsize);
	if (!mem)
	{
		return (NULL);
	}
	ft_bzero(mem, nelem * elsize);
	return (mem);
}
