/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcalleja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 15:35:43 by mcalleja          #+#    #+#             */
/*   Updated: 2026/06/02 18:51:57 by mcalleja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t n, size_t size)
{
	void	*memo;

	if (size != 0 && n > ((size_t)-1) / size)
		return (NULL);
	memo = (void *)malloc(n * size);
	if (!memo)
		return (0);
	ft_bzero(memo, (n * size));
	return (memo);
}
